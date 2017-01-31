#ifndef ORGANISM_H_INCLUDED
#define ORGANISM_H_INCLUDED
#include <list>
#include <algorithm>
#include <iomanip>

#include "Shapes.h"
#include "util.h"
#include "Networking.h"

class organism;

class brain
{
    public:
    organism *owner;
    sf::Vector2f desiredPos;
    bool desiresMate;
    brain();
};

class organism
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
    brain *brain;
    sf::Color colorPrime;
    sf::Color colorSecondary;


    float ageMax;
    float age;
    float gestationPeriod;
    float gestationTime;
    organism();
    float getHealthMax();
    float getSpeed();
    float getNutritionMax();
    float getHydrationMax();
};


extern std::list<brain> BrainStorage;
extern std::list<organism> Organisms;
extern std::list<organism> Flora;

void runBrains();

void worldPopulationSetup();

void addCreatures(int amount);

void displayCrittersInfo();
void drawCritters();



#endif // ORGANISM_H_INCLUDED
