#include "Organism.h"

std::list<organism> Organisms;

class organism
{
public:
    float health;
    float baseSpeed;
    float size;
    float nutrition;
    float hydration;

    float ageMax;
    float age;
    float gestationPeriod;
    float gestationTime;


    organism()
    {
        size = random(1,100) * 0.1;
        baseSpeed = random(1,10);

        health = getHealthMax();
        nutrition = getNutritionMax();
        hydration = getHealthMax();
    }

    float getHealthMax()
    {
        return std::max(size * 2,1.f);
    }

    float getSpeed()
    {
        return baseSpeed / size;
    }

    float getNutritionMax()
    {
        return std::max(size / 2,1.f);
    }

    float getHydrationMax()
    {
        return std::max(size,1.f);
    }



};

void worldPopulationSetup()
{
    for(int i = 0; i != 10; i++)
    {
        organism Critter;
        Organisms.push_back(Critter);
    }
}

void displayCritters()
{
    int yOffset = 1;
    for(auto &crit : Organisms)
    {
        std::string descString = "Critter; (" + std::to_string(int(crit.health)) +
            "), " + std::to_string(int(crit.nutrition)) + ":" + std::to_string(int(crit.hydration));

        shapes.createText(5,10*yOffset,10,sf::Color::Cyan, descString);
        yOffset++;
    }
}
