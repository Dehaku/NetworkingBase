#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "Organism.h"
#include "Tiles.h"



class Simulation
{
public:
    std::list<Brain> brainStorage;
    std::list<Organism> flora;
    std::vector<Organism> organisms;
    std::vector<std::vector<WorldTile>> worldTiles;
    void worldPopulationSetup();

};

class SimulationManager
{
public:
    int simulationID;
    std::list<Simulation> simulations;
    Simulation* createSimulation();

    SimulationManager();

};
extern SimulationManager simulationManager;


#endif // SIMULATION_H_INCLUDED
