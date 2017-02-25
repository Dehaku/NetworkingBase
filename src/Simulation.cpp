#include "Simulation.h"

SimulationManager simulationManager;

void Simulation::runLife()
{
    runCreatures(organisms);
    runPlants(flora);
    //runBrains(organisms);
}

Simulation::Simulation()
{
    worldTileSizeX = 1000;
    worldTileSizeY = 1000;

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
        Critter->sim = this;
        Critter->name = generateName(1,3);
        Critter->pos = sf::Vector2f(random(10,1000),random(10,1000));
        organisms.push_back(Critter);

        //Brain creatureBrain;
        std::shared_ptr<Brain> creatureBrain(new Brain());
        brainStorage.push_back(creatureBrain);

        organisms.back().get()->brain = brainStorage.back();
        brainStorage.back().get()->owner = organisms.back();
    }
}

void Simulation::drawWorld()
{

    int xOffset = 0;
    int yOffset = 0;
    for(int i = 0; i != worldTiles.size(); i++)
    {
        for(int t = 0; t != worldTiles[i].size(); t++)
        {
            shapes.createSquare(i*worldTileSizeX,t*worldTileSizeY,i*worldTileSizeX+worldTileSizeX,t*worldTileSizeY+worldTileSizeY,worldTiles[i][t].color);
            xOffset++;
        }
        yOffset++;
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

    drawWorld();



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

Simulation* SimulationManager::getCurrentSimulation()
{
    int i = 0;
    for(auto &sim : simulationManager.simulations)
    {
        if(i == simulationManager.drawSimNumber)
        {
            return &sim;
        }
        i++;
    }
    return nullptr;
}




// ==================== Old Organism.cpp File below ====================



std::list<Brain> brainStorage;
std::list<Organism> flora;
std::list<Organism> organisms;

Brain::Brain()
{
    desiresMate = random(0,1);
}




Organism::Organism()
{
    age = 0;
    ageMax = 30000;
    gestationTime = 0;
    gestationPeriod = 5000;

    baseSpeed = random(1,10);

    //* brain = nullptr;
    colorPrime = sf::Color(random(0,255),random(0,255),random(0,255));
    colorSecondary = sf::Color(random(0,255),random(0,255),random(0,255));

    health = getHealthMax();
    nutritionMax = random(50,100);
    hydrationMax = random(50,200);

    nutrition = getNutritionMax();
    hydration = getHydrationMax();

    // Initial Traits
    {


        Trait dirtEater;
        dirtEater.type = TraitID::Detritivore;
        dirtEater.vars.push_back(0.05f);
        traits.push_back(dirtEater);

        int ran = random(1,10);

        if(ran == 2)
        {
            Trait carnivore;

            carnivore.type = TraitID::Carnivore;
            carnivore.vars.push_back(1.f);
            carnivore.vars.push_back(10.f);
            traits.push_back(carnivore);
        }
        else if(ran == 1)
        {
            Trait herbivore;
            herbivore.type = TraitID::Herbivore;
            herbivore.vars.push_back(1.f);
            herbivore.vars.push_back(10.f);
            traits.push_back(herbivore);
        }



    }
}

float Organism::getHungerRate()
{
    float hungerRate = 0.01;

    hungerRate += 0.01*size;
    //hungerRate += 0.01*strength;



    hungerRate += 0.01*baseSpeed;

    // If pregnant/making baby, double hunger.
    if(gestationTime > 0)
        hungerRate = hungerRate*2;

    return hungerRate;
}

float Organism::getThirstRate()
{
    float thirstRate = 0.01;

    thirstRate += 0.01*size;
    //thirstRate += 0.01*strength;
    thirstRate += 0.01*baseSpeed;

    return thirstRate;
}

void Organism::runHealth()
{
    health = std::min(health+0.01f,getHealthMax());
}

void Organism::runHunger()
{



    // Nutrition lost should be based on size, strength, and speed. (As well as special traits)
    nutrition -= getHungerRate();

    // Traits
    for(auto &trait : traits)
    {
        if(trait.type == TraitID::Detritivore)
        {
            if(inputState.key[Key::I])
                nutrition = std::min(nutrition+trait.vars[0]*50,getNutritionMax());
            else
                nutrition = std::min(nutrition+trait.vars[0],getNutritionMax());
        }
    }

    if(nutrition < 0)
    {
        nutrition = 0;
        health--;
    }
}

void Organism::runHydration()
{
    // hydration -= getThirstRate();
    if(hydration < 0)
    {
        hydration = 0;
        health--;
    }
}

void Organism::runGestation()
{
    // Placeholder until breeding is trait based.
    {
        // Alot of these will be modified by traits, like fed well, and healthy, ect.
        bool notGrowing = (gestationTime == 0); // If not already pregnant. (This is to make sure gestationTime doesn't keep getting set to 1)
        bool oldEnough = (age > ageMax*0.33); // Sexual maturity occurs at a third of it's lifespan.
        bool fedWell = (nutrition > getNutritionMax()*0.8); // Needs healthy diet
        bool healthy = (health > getHealthMax() * 0.75); // Needs to be in decent health.
        if(notGrowing && oldEnough && fedWell && healthy)
            gestationTime = 1;
    }



    if(gestationTime > 0) // Getting pregnant or whatever will set this timer to 1, kicking this off.
        gestationTime++;

    if(gestationTime >= gestationPeriod)
    {
        gestationTime = 0;
        giveBirth();
    }
}

void Organism::giveBirth()
{
    // Check traits for mitosis/egg/sudden desire for parasite injection/ect.

    std::cout << sim->organisms.size() << std::endl;
    for(int i = 0; i != 1; i++)
    {
        std::shared_ptr<Organism> Critter(new Organism());

        *(Critter.get()) = *this; // Give it all the same information.
        Critter->name = name+generateName(1,1); // Enhance name slightly.
        sim->organisms.push_back(Critter);
        // sim->organisms.back().get() = this;

        std::shared_ptr<Brain> creatureBrain(new Brain());
        sim->brainStorage.push_back(creatureBrain);

        sim->organisms.back().get()->brain = sim->brainStorage.back();
        sim->brainStorage.back().get()->owner = sim->organisms.back();
    }


}


float Organism::getHealthMax()
{
    return std::max(size * 2,1.f);
}

float Organism::getSpeed()
{
    return baseSpeed / size;
}

float Organism::getNutritionMax()
{
    return std::max(float(nutritionMax*size),1.f);
}

float Organism::getHydrationMax()
{
    return std::max(float(hydrationMax*size),1.f);
}



void moveAngle(Organism &crit, float ang)
{

    crit.pos.x += cosf(ang) * crit.getSpeed();
    crit.pos.y += sinf(ang) * crit.getSpeed();
}

void runBrain(Organism &crit)
{
    if(crit.health < 1) // ded.
        return;

    if(!network::client)
        if(random(1,600) == 1 || inputState.key[Key::Space].time == 1)
            crit.brain.lock()->desiredPos = sf::Vector2f(random(10,990),random(10,990));


    //std::cout << "Critter" + crit.name + ": " + std::to_string(crit.getSpeed()) << std::endl;
    moveAngle(crit,math::angleBetweenVectors(crit.pos,crit.brain.lock()->desiredPos));


}

//void runBrains(std::list<Organism>& organismList)
void runBrains(std::list<std::shared_ptr<Organism>>& organismList)
{
    for(auto &crit : organismList)
        runBrain(*crit.get());
}

void runBody(Organism &crit)
{
    crit.age++;

    crit.runHealth();
    crit.runHunger();
    crit.runHydration();
    crit.runGestation();
}

void runCreatures(std::list<std::shared_ptr<Organism>>& organismList)
{
    for(auto &crit : organismList)
    {
        runBody(*crit.get());

        runBrain(*crit.get());
    }

}

void runPlants(std::list<std::shared_ptr<Organism>>& organismList)
{
    for(auto &plant : organismList)
    {
        plant.get()->size += 0.001;


        // runBrain(*crit.get());
    }

}

void worldPopulationSetup()
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

        //* organisms.back().brain = &brainStorage.back();
        //* brainStorage.back().owner = &organisms.back();
    }
}

void addCreatures(int amount)
{
    for(int i = 0; i != amount; i++)
    {
        Organism critter;
        critter.name = std::to_string(i); // Temporary
        critter.pos = sf::Vector2f(random(10,1000),random(10,1000));
        organisms.push_back(critter);
        Brain creatureBrain;
        brainStorage.push_back(creatureBrain);

        //* organisms.back().brain = &brainStorage.back();
        //* brainStorage.back().owner = &organisms.back();
    }
}

/*
FPS Tests
10 - 10000 Pop Cap
Disable Drawing of Creatures
Disable drawing of Info Popups
(Disable Graphics for each, basically)

Attempt multithreading of creatures. Perhaps splitting the list into four equal parts, and running them concurrently?
Quad-trees may be useful once creatures start to reference eachother.


*/

void displayCrittersInfo()
{
    int yOffset = 21;










    shapes.createText(-130,10*yOffset,10,sf::Color::White, "World Pop:" + std::to_string(organisms.size()), &gvars::hudView);
    yOffset++;
    //for(int i = 0; i != Organisms.size(); i++)
    int limitCounter = 0;
    for(auto &crit : organisms)
    {
        if(limitCounter > 10)
            break;
        std::string descString = "Critter; (" + std::to_string(int(crit.health)) +
            "), " + std::to_string(int(crit.nutrition)) + ":" + std::to_string(int(crit.hydration));

        shapes.createText(-130,10*yOffset,10,sf::Color::Cyan, descString, &gvars::hudView);
        yOffset++;
        limitCounter++;
    }


}



void drawCritters()
{
    static bool draw = true;
    static bool drawSquareInstead = false;
    static bool drawTextureInstead = false;
    static sf::Color background(150,150,10);

    static sf::Sprite circleCritter;
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

    for(auto &plant : flora)
    {

        shapes.createCircle(plant.pos.x,plant.pos.y,plant.size,plant.colorPrime,plant.size/10,plant.colorSecondary);
    }

    sf::Texture &circley = texturemanager.getTexture("Circle.png");
    sf::Texture &swirl = texturemanager.getTexture("SwirlEffect.png");
    static int rotation = 0;
    if(inputState.key[Key::Z])
        rotation++;

    for(auto &crit : organisms)
    {
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




