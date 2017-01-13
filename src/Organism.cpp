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
        size = random(1,10);
        baseSpeed = random(1,10);

        health = getHealthMax();
        nutrition = getNutritionMax();
        hydration = getHealthMax();
    }

    float getHealthMax()
    {
        return size * 2;
    }

    float getSpeed()
    {
        return baseSpeed / size;
    }

    float getNutritionMax()
    {
        return size / 2;
    }

    float getHydrationMax()
    {
        return size;
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
