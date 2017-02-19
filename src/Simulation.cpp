#include "Simulation.h"

SimulationManager simulationManager;

Simulation::Simulation()
{
    simulationID = 0;
    draw = true;
    drawSquareInstead = false;
    drawTextureInstead = false;
    background = sf::Color(150,150,10);
    circleCritter;
}

void Simulation::worldPopulationSetup()
{
    for(int i = 0; i != 100; i++)
    {
        //Organism Plant;
        std::shared_ptr<Organism> Plant(new Organism());
        Plant->size = random(5,30);
        Plant->baseSpeed = 0;
        Plant->pos = sf::Vector2f(random(10,1000),random(10,1000));
        Plant->colorPrime = sf::Color(0,200,0,150);
        Plant->colorSecondary = sf::Color(0,100,0,150);

        flora.push_back(Plant);
    }

    for(int i = 0; i != 100; i++)
    {
        //Organism Critter;
        std::shared_ptr<Organism> Critter(new Organism());
        Critter->name = std::to_string(i); // Temporary
        Critter->pos = sf::Vector2f(random(10,1000),random(10,1000));
        organisms.push_back(Critter);

        //Brain creatureBrain;
        std::shared_ptr<Brain> creatureBrain(new Brain());
        brainStorage.push_back(creatureBrain);

        organisms.back().get()->brain = brainStorage.back();
        brainStorage.back().get()->owner = organisms.back();
    }
}

void Simulation::drawCritters()
{
    if(circleCritter.getTexture() == nullptr)
        circleCritter.setTexture(texturemanager.getTexture("Circle.png"));

    if(inputState.key[Key::Tab].time == 1)
        toggle(draw);
    if(inputState.key[Key::R].time == 1)
        toggle(drawSquareInstead);
    if(inputState.key[Key::T].time == 1)
        toggle(drawTextureInstead);

    if(!draw)
        return;

    shapes.createSquare(0,0,1000,1000,background);

    for(auto &planty : flora)
    {
        Organism &plant = *planty.get();
        shapes.createCircle(plant.pos.x,plant.pos.y,plant.size,plant.colorPrime,plant.size/10,plant.colorSecondary);
    }

    sf::Texture &circley = texturemanager.getTexture("Circle.png");
    sf::Texture &swirl = texturemanager.getTexture("SwirlEffect.png");

    for(auto &critter : organisms)
    {
        Organism &crit = *critter.get();

        if(drawSquareInstead)
            shapes.createSquare(crit.pos.x-((crit.size)),crit.pos.y-((crit.size)),crit.pos.x+((crit.size)),crit.pos.y+((crit.size)),crit.colorPrime,crit.size/2,crit.colorSecondary);
        else if(drawTextureInstead)
        {
            shapes.createImageButton(crit.pos,circley);
            //shapes.createImageButton(crit.pos,swirl,"",rotation);
        }
        else
            shapes.createCircle(crit.pos.x,crit.pos.y,crit.size,crit.colorPrime,crit.size/2,crit.colorSecondary);
    }


    if(inputState.key[Key::Space].time == 1 && true == false)
    {
        background = sf::Color(random(0,255),random(0,255),random(0,255));
        std::cout << "Color: " << background.r << "/" << background.g << "/" << background.b << std::endl;
        std::string descString = "Color: " + std::to_string(background.r) + "/" + std::to_string(background.g) + "/" + std::to_string(background.b);
        //shapes.createText(100,100,10,sf::Color::White, descString);

    }


}

SimulationManager::SimulationManager()
{
    simulationID = 0;
    drawSimNumber = 0;
}

Simulation* SimulationManager::createSimulation()
{
    simulationID++;
    std::cout << "Creating Simulation " << simulationID << std::endl;
    Simulation simulation;
    simulations.push_back(simulation);
    // You must use .back() here, and work it /after/ it's in the container, otherwise the pointers between the containers inside simulation will fail.
    // I should probably use 'new' and have a list of pointers, instead.
    simulations.back().simulationID = simulationID;
    simulations.back().worldPopulationSetup();
    worldTilesSetup(simulations.back().worldTiles);

    std::cout << "Critters: " << simulations.back().organisms.size() << std::endl;


    return &simulations.back();
}

Simulation* SimulationManager::getSimulation(int id)
{

    for(auto &sim : simulations)
        if(sim.simulationID == id)
            return &sim;

    return nullptr;
}
