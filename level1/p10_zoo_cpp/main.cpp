#include "zoo.h"
#include "animals.h"

int main() {
    Zoo zoo;

    zoo.addAnimal(std::make_unique<Dog>());
    zoo.addAnimal(std::make_unique<Cat>());
    zoo.speakAll();
    printf("\n");

    zoo.addAnimal(std::make_unique<Bird>());
    zoo.addAnimal(std::make_unique<WolfDog>());
    zoo.speakAll();
    return 0;
}