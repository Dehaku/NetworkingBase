#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include "Organism.h"

class Simulation
{
public:
    std::vector<Organism> organisms;
};

class SimulationManager
{
public:
    int simulationID;
    SimulationManager();

};


#endif // SIMULATION_H_INCLUDED
