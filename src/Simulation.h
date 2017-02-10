#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "Organism.h"
#include "Tiles.h"



class Simulation
{
public:
    std::list<std::shared_ptr<Brain>> brainStorage;
    std::list<std::shared_ptr<Organism>> flora;
    std::list<std::shared_ptr<Organism>> organisms;
    std::vector<std::vector<WorldTile>> worldTiles;

    bool draw;
    bool drawSquareInstead;
    bool drawTextureInstead;
    sf::Color background;
    sf::Sprite circleCritter;

    Simulation();


    void worldPopulationSetup();

    void drawCritters();

};

class SimulationManager
{
public:
    int simulationID;
    int maxSimulationsAllowed;
    int drawSimNumber;
    std::list<Simulation> simulations;
    Simulation* createSimulation();

    SimulationManager();

};
extern SimulationManager simulationManager;


#endif // SIMULATION_H_INCLUDED
