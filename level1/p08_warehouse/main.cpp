#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

std::map<std::string, int> inventory;

void loadInventory(const std::string& filename) {
    std::ifstream fin(filename);

    if (!fin.is_open()) {
        printf("Unable to open file for reading.\n");
        return;
    }

    std::string line;
    int lineNo = 0;

    while (std::getline(fin, line)) {
        ++lineNo;

        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string model;
        int quantity;

        if (!(iss >> model >> quantity)) {
            printf("Line %i has a format error and has been skipped.\n", lineNo);
            continue;
        }
        if (quantity <= 0) {
            printf("Line %i has a negative or zero quantity and has been skipped.\n", lineNo);
            continue;
        }

        inventory[model] += quantity;
    }

    fin.close();

    printf("Inventory loaded successfully.\n");
}

void printMenu() {
    printf("-----Menu-----\n");
    printf("1---Show inventory list\n");
    printf("2---Stock in\n");
    printf("3---Stock out\n");
    printf("4---Exit program\n");
    printf("Enter the corresponding number to select a menu function.\n");
}

void showInventory() {
    for (const auto& pair : inventory) {
        printf("%s ", pair.first.c_str());
        printf("%i\n", pair.second);
    }
}

void stockIn(const std::string& model, int quantity) {
    inventory[model] += quantity;
}

bool stockOut(const std::string& model, int quantity) {
    auto it = inventory.find(model);
    if (it != inventory.end() && it->second >= quantity) {
        it->second -= quantity;
        return true;
    }
    return false;
}

void saveInventory(const std::string& filename) {
    std::ofstream fout(filename);

    if (!fout.is_open()) {
        printf("Unable to open file for writing.\n");
        return;
    }

    for (const auto& pair : inventory) {
        fout << pair.first << ' ' << pair.second << '\n';
    }

    fout.close();
    printf("Saved successfully.\n");
}

int main() {
    int mode = 0;
    std::string model;
    char m[64];
    int quantity = 0;
    const std::string filename = "inventory.txt";
    loadInventory(filename);
    while (1) {
        printMenu();
        scanf("%i", &mode);
        if (mode < 1 || mode > 4) {
            printf("Invalid input\n");
        }
        else if (mode == 1) {
            showInventory();
        }
        else if (mode == 2) {
            printf("Enter the product model and quantity.\n");
            scanf("%63s %i", m, &quantity);
            if (quantity <= 0) {
                printf("Invalid input\n");
            }
            else {
                model = m;
                stockIn(model, quantity);
                printf("Stock in %s %i\n", model.c_str(), quantity);
            }
        }
        else if (mode == 3) {
            printf("Enter the product model and quantity.\n");
            scanf("%63s %i", m, &quantity);
            if (quantity <= 0) {
                printf("Invalid input\n");
            }
            else {
                model = m;
                bool flag = stockOut(model, quantity);
                if (flag) {
                    printf("Stock out %s %i\n", model.c_str(), quantity);
                }
                else {
                    printf("Product not found or insufficient stock.\n");
                }
            }
        }
        else if (mode == 4) {
            break;
        }
    }
    saveInventory(filename);
    return 0;
}