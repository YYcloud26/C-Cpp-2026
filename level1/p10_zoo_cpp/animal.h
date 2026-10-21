#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>

class Animal {
public:
    virtual ~Animal() = default;

    virtual void speak() const = 0;

    virtual std::string name() const = 0;
};

#endif