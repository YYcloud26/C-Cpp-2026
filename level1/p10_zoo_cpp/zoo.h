#ifndef ZOO_H
#define ZOO_H

#include <vector>
#include <memory>
#include <iostream>
#include "animal.h"

class Zoo {
public:
    void addAnimal(std::unique_ptr<Animal> animal) {
        animals_.push_back(std::move(animal));
    }

    void speakAll() const {
        for (const auto& a : animals_) {
            std::cout << a->name() << ":";
            a->speak();
            std::cout << "\n";
        }
    }

    std::size_t size() const { return animals_.size(); }

private:
    std::vector<std::unique_ptr<Animal>> animals_;
};

#endif