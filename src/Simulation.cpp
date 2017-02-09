#include "Simulation.h"

SimulationManager simulationManager;

void Simulation::worldPopulationSetup()
{
    for(int i = 0; i != 100; i++)
    {
        Organism Plant;
        Plant.size = random(5,30);
        Plant.baseSpeed = 0;
        Plant.pos = sf::Vector2f(random(10,1000),random(10,1000));
        Plant.colorPrime = sf::Color(0,200,0,150);
        Plant.colorSecondary = sf::Color(0,100,0,150);
        flora.push_back(Plant);
    }

    for(int i = 0; i != 100; i++)
    {
        Organism Critter;
        Critter.name = std::to_string(i); // Temporary
        Critter.pos = sf::Vector2f(random(10,1000),random(10,1000));
        organisms.push_back(Critter);
        Brain creatureBrain;
        brainStorage.push_back(creatureBrain);

        organisms.back().brain = &brainStorage.back();
        brainStorage.back().owner = &organisms.back();
    }
}

SimulationManager::SimulationManager()
{
    simulationID = 0;
}

Simulation* SimulationManager::createSimulation()
{
    Simulation simulation;
    simulations.push_back(simulation);

    return &simulations.back();
}
