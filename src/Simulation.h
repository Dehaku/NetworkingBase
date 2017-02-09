#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "Organism.h"
#include "Tiles.h"



class Simulation
{
public:
    std::list<Brain> brainStorage;
    std::list<Organism> flora;
    std::list<Organism> organisms;
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
    std::list<Simulation> simulations;
    Simulation* createSimulation();

    SimulationManager();

};
extern SimulationManager simulationManager;


#endif // SIMULATION_H_INCLUDED
