#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "Organism.h"
#include "Tiles.h"

class Brain;
class Organism;

class Simulation
{
public:
    bool toDelete;
    unsigned int simulationID;
    unsigned int populationID;
    unsigned int populationAll;
    unsigned int populationAlive;
    unsigned int populationDead;

    std::list<std::shared_ptr<Brain>> brainStorage;
    std::list<std::shared_ptr<Organism>> flora;
    std::list<std::shared_ptr<Organism>> organisms;
    void syncOrganism(std::shared_ptr<Organism> critter);
    std::vector<std::vector<WorldTile>> worldTiles;
    unsigned int worldTileSizeX;
    unsigned int worldTileSizeY;

    bool paused;

    bool draw;
    bool drawSquareInstead;
    bool drawTextureInstead;
    sf::Color background;
    sf::Sprite circleCritter;

    void runLife();
    Simulation();


    void worldPopulationSetup();

    void drawWorld();
    void drawCritters();

};

class SimulationManager
{
public:
    unsigned int simulationID;
    int maxSimulationsAllowed;
    int drawSimNumber;
    std::list<Simulation> simulations;

    void runSimulations();

    Simulation* createSimulation();
    Simulation* getSimulation(int id);
    Simulation* getCurrentSimulation();

    SimulationManager();

};
extern SimulationManager simulationManager;




// ==================== Old Organism.h File below ====================



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
    bool toDelete;
    Trait();
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

    /*

    *Sync Checklist*
    Organism Packet
    Mutation
    Blueprint

    */
    public:
    Simulation* sim;
    unsigned int ID;
    bool toDelete;

    sf::Vector2f pos;
    float health;
    float healthMax;
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
    float gestationPeriodBase;
    float gestationTime;

    Organism();
    void mutate(Organism* secondParent);
    float getHealthMax();
    float getSpeed();
    float getNutritionMax();
    float getHungerRate();
    float getHydrationMax();
    float getThirstRate();
    float getGestationPeriod();

    bool isDead();
    bool isHungry();
    bool isStillHungry;
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





#endif // SIMULATION_H_INCLUDED
