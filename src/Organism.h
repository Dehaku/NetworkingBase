#ifndef ORGANISM_H_INCLUDED
#define ORGANISM_H_INCLUDED
#include <list>
#include <algorithm>
#include <iomanip>
#include <memory>

#include "Shapes.h"
#include "util.h"
#include "Networking.h"

namespace TraitID
{
    enum
    {
        Nothing,
        Detritivore,
        Herbivore,
        Carnivore
    };
}

class Trait
{
public:
    int type;
    std::vector<float> vars;
};



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
    sf::Vector2f pos;
    float health;
    float baseSpeed;
    float size;
    float nutrition;
    float nutritionMax;
    float hydration;
    float hydrationMax;

    // Identity
    std::string name;
    std::weak_ptr<Brain> brain;
    sf::Color colorPrime;
    sf::Color colorSecondary;
    std::vector<Trait> traits;


    float ageMax;
    float age;
    float gestationPeriod;
    float gestationTime;

    Organism();
    float getHealthMax();
    float getSpeed();
    float getNutritionMax();
    float getHungerRate();
    float getHydrationMax();
    float getThirstRate();

    void runHealth();
    void runHunger();
    void runHydration();
    void runGestation();
    void giveBirth(); // The money maker, right here.


};


extern std::list<Brain> brainStorage;
extern std::list<Organism> organisms;
extern std::list<Organism> flora;

void runBrains(std::list<std::shared_ptr<Organism>>& organismList);
void runCreatures(std::list<std::shared_ptr<Organism>>& organismList);
void runPlants(std::list<std::shared_ptr<Organism>>& organismList);


void worldPopulationSetup();

void addCreatures(int amount);

void displayCrittersInfo();
void drawCritters();



#endif // ORGANISM_H_INCLUDED
