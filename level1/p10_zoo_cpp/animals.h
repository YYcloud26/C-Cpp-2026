#ifndef ANIMALS_H
#define ANIMALS_H

#include <iostream>
#include <string>
#include "animal.h"

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!"; }
    std::string name() const override { return "Dog"; }
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Meow--"; }
    std::string name() const override { return "Cat"; }
};

class Bird : public Animal {
public:
    void speak() const override { std::cout << "Tweet!"; }
    std::string name() const override { return "Bird"; }
};

class WolfDog : public Dog {
public:
    void speak() const override { std::cout << "Awooo--"; }
    std::string name() const override { return "Wolfdog"; }
};

#endif