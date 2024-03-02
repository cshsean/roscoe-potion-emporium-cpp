#include <array>
#include <vector>
#include <iostream>
#include <string_view>
#include <string>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "random.h"

bool QUIT{ false };

namespace Potion {
    enum Type {
        healing,
        mana,
        speed,
        invisibility,
        max_types
    };

    std::array<int, Potion::max_types> const costs{ 20, 30, 12, 50 };
    std::array<std::string_view, Potion::max_types> const names{ "healing", "mana", "speed", "invisibility" };
}

class Player {
private:
    const std::string m_name{};
    int m_gold{};
    std::vector<int> m_inventoryItem{};
    std::vector<int> m_inventoryNo{};

public:
    explicit Player(std::string_view name)
        : m_name{ name }
        , m_gold{ Random::get(80, 120) }
    {}

    std::string_view    getName() const { return m_name; };
    int                 getGold() const { return m_gold; };
    void                deductGold(int gold) { m_gold -= gold; };
    friend void         stateInitialGoldAmount(const Player& p);
    friend void         showInventory(const Player& p);
    bool                checkIfItemInInventory(int item) {
                            return (std::find(m_inventoryItem.begin(), m_inventoryItem.end(), item) != m_inventoryItem.end()) ? true : false;
                        }

    void                addToInventoryNo(int potionChoice, int itemNo) {
                            // If item not in inventory
                            if (itemNo == 0) {
                                m_inventoryItem.push_back(potionChoice);
                                m_inventoryNo.push_back(1);
                            }
                            // If item in inventory
                            else {
                                for (int index{ 0 }; index < static_cast<int>(m_inventoryItem.size()); ++index) {
                                    if (potionChoice == m_inventoryItem[index]) {
                                        ++m_inventoryNo[index];
                                    }
                                }
                            }
                        }
};

template <typename T, typename U, auto N>
void shop(const std::array<T,N>& costs, const std::array<U,N>& names) {
    std::cout << "Here is our selection for today: \n";
    for (int index{ 0 }; index < Potion::max_types; ++index) {
        std::cout << index << ") " << names[index] << " costs " << costs[index] << '\n';
    }
}

std::string initiateName() {
    std::string name{};
    std::cout << "Welcome to Roscoe's potion emporium!\n";
    std::cout << "Enter your name: ";
    std::cin >> name;
    return name;
}

void stateInitialGoldAmount(const Player& p) {
    // assert(p.m_name == "Alex");
    std::cout << "Hello, " << p.m_name << ", you have " << p.m_gold << " gold.\n\n";
}

void showInventory(const Player& p) {
    std::cout << "\nYour inventory contains:\n"; 
    for (int index{ 0 }; index < static_cast<int>(p.m_inventoryItem.size()); ++index) {
        std::cout << p.m_inventoryNo[index] << "x potion of " << Potion::names[p.m_inventoryItem[index]] << '\n';
    }
    std::cout << "You escaped with " << p.m_gold << " gold remaining.\n";
}

int main()
{
    Player p{ initiateName() };

    stateInitialGoldAmount(p);

    while (p.getGold() > 0 && !QUIT) { 
        bool reattempt{ false };
        while (true) {

            char potionChoiceChar{};

            if (reattempt) {
                std::cout << "  Try again: ";
            }
            else {
                shop(Potion::costs, Potion::names);
                std::cout << "Enter the number of the potion you'd like to buy, or 'q' to quit: ";
            }

            std::cin >> potionChoiceChar;

            if (potionChoiceChar == 'q') { QUIT = true; break; };

            int potionChoiceInt = potionChoiceChar - '0';

            if ((!potionChoiceInt && potionChoiceInt != 0) || potionChoiceInt < 0 || potionChoiceInt >(Potion::max_types - 1))
            {
                reattempt = true;
                std::cin.clear();
                std::cout << "That is not a valid input.";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            else { reattempt = false; }

            if (p.getGold() < Potion::costs[potionChoiceInt]) {
                std::cout << "You cannot afford that.\n\n";
                break;
            }

            p.deductGold(Potion::costs[potionChoiceInt]);

            bool isItemInInventory{ p.checkIfItemInInventory(potionChoiceInt) };

            isItemInInventory ? p.addToInventoryNo(potionChoiceInt, 1) : p.addToInventoryNo(potionChoiceInt, 0);

            std::cout << "You purchased a potion of " << Potion::names[potionChoiceInt] << ". You have " << p.getGold() << " gold left.\n\n";

            /*
                - If cost of item > gold, reject and continue
                - Deduct the number of gold
                After buying item, store type of item in m_inventoryItem
                    If item not in m_inventoryItem, add 0 to respective item in m_inventoryNo
                    If item in m_inventoryItem, ++m_inventoryNo[index of item]
            */
        }
    }

    showInventory(p);

    std::cout << "\nThanks for shopping at Roscoe's potion emporium!\n";

    return 0;
}
