#ifndef ORGANISM_H_INCLUDED
#define ORGANISM_H_INCLUDED
#include <list>
#include <algorithm>
#include <iomanip>
#include <memory>

#include "Shapes.h"
#include "util.h"
#include "Networking.h"

class Organism;

class Brain
{
    public:
    std::weak_ptr<Organism> owner;
    sf::Vector2f desiredPos;
    bool desiresMate;
    Brain();
};

class Organism
{
    public:
    public:
    sf::Vector2f pos;
    float health;
    float baseSpeed;
    float size;
    float nutrition;
    float hydration;

    // Identity
    std::string name;
    std::weak_ptr<Brain> brain;
    sf::Color colorPrime;
    sf::Color colorSecondary;


    float ageMax;
    float age;
    float gestationPeriod;
    float gestationTime;
    Organism();
    float getHealthMax();
    float getSpeed();
    float getNutritionMax();
    float getHydrationMax();
};


extern std::list<Brain> brainStorage;
extern std::list<Organism> organisms;
extern std::list<Organism> flora;

void runBrains(std::list<std::shared_ptr<Organism>>& organismList);

void worldPopulationSetup();

void addCreatures(int amount);

void displayCrittersInfo();
void drawCritters();



#endif // ORGANISM_H_INCLUDED
