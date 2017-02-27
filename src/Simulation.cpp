#include "Simulation.h"

SimulationManager simulationManager;


void runDeadColors()
{
    for(auto &sim : simulationManager.simulations)
        for(auto &critter : sim.organisms)
        {
            Organism& crit = *critter.get();
            if(crit.isDead())
            {
                crit.colorPrime.r = std::min(crit.colorPrime.r+1,255);
                crit.colorPrime.g = std::min(crit.colorPrime.g+1,255);
                crit.colorPrime.b = std::min(crit.colorPrime.b+1,255);

                crit.colorSecondary.r = std::min(crit.colorSecondary.r+1,255);
                crit.colorSecondary.g = std::min(crit.colorSecondary.g+1,255);
                crit.colorSecondary.b = std::min(crit.colorSecondary.b+1,255);

                if(crit.age > crit.ageMax*2.75)
                {
                    crit.colorPrime.a = std::max(crit.colorPrime.a-1,0);
                    crit.colorSecondary.a = std::max(crit.colorSecondary.a-1,0);
                }
            }
        }
}


void Simulation::runLife()
{
    runCreatures(organisms);
    runPlants(flora);

    unsigned int aliveCount = 0;
    unsigned int deadCount = 0;

    for(auto &crit : organisms)
    {

        if(!crit.get()->isDead())
            aliveCount++;
        else
            deadCount++;

    }

    populationAlive = aliveCount;
    populationDead = deadCount;


    AnyDeletesSmartPointer(organisms);
    //runBrains(organisms);
}

Simulation::Simulation()
{
    populationID = 0;
    populationAll = 0;
    populationAlive = 0;
    populationDead = 0;

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
        Plant->ID = populationID++;
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
        Critter->ID = populationID++;
        organisms.push_back(Critter);

        //Brain creatureBrain;
        std::shared_ptr<Brain> creatureBrain(new Brain());
        brainStorage.push_back(creatureBrain);
        populationAll++;

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

void SimulationManager::runSimulations()
{
    for(auto &sim : simulations)
        sim.runLife();

    if( (fpsKeeper.updatesPassedTotal % 10) == 0 )
    {
        runDeadColors();
    }
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
    toDelete = false;

    age = 0;
    ageMax = 30000;
    gestationTime = 0;
    gestationPeriod = 5000;

    size = random(10,100) * 0.1;
    baseSpeed = random(1,10);

    //* brain = nullptr;
    colorPrime = sf::Color(random(0,255),random(0,255),random(0,255));
    colorSecondary = sf::Color(random(0,255),random(0,255),random(0,255));

    health = getHealthMax();
    nutritionMax = random(50,100);
    hydrationMax = random(50,200);

    nutrition = getNutritionMax();
    hydration = getHydrationMax();
    isStillHungry = false;

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
            herbivore.vars.push_back(10.f); // Var 0, How much nutrition is gained per nom.
            herbivore.vars.push_back(0.1f); // Var 1, How much plant size is consumed per nom.
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

bool Organism::isDead()
{
    if(age > ageMax*3) // Super Dead, Time to go byebye.
        toDelete = true;

    bool amIDead = false;
    if(health <= 0)
        return true;

    if(age > ageMax)
        return true;


    return amIDead;
}

bool Organism::isHungry()
{
    bool returnStatus = false;

    if(nutrition > getNutritionMax()*0.9)
        isStillHungry = false;

    if(nutrition < getNutritionMax()*0.5)
    {
        isStillHungry = true;
        returnStatus = true;
    }

    if(isStillHungry)
        returnStatus = true;

    return returnStatus;
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
        sim->organisms.push_back(Critter);
        sim->populationAll++;

        std::shared_ptr<Brain> creatureBrain(new Brain());
        sim->brainStorage.push_back(creatureBrain);

        sim->organisms.back().get()->brain = sim->brainStorage.back();
        sim->brainStorage.back().get()->owner = sim->organisms.back();

        Organism& critter = *(sim->organisms.back().get());

        critter.ID = sim->populationID++;
        critter.age = 0;
        critter.name.append(generateName(1,1));
        critter.nutrition = critter.getNutritionMax();
        critter.hydration = critter.getHydrationMax();
        critter.health = critter.getHealthMax();


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

float length(sf::Vector2f Vec){
    return sqrt(Vec.x*Vec.x+Vec.y*Vec.y);
 }

void normalize(sf::Vector2f &Vec){
     float len = length(Vec);
     Vec.x /= len;
     Vec.y /= len;
}



void moveVector(Organism &crit, sf::Vector2f vPos)
{
    sf::Vector2f diffVec(vPos-crit.pos);
    normalize(diffVec);
    diffVec *= crit.getSpeed();
    crit.pos += diffVec;

    //crit.pos.x += cosf(ang) * crit.getSpeed();
    //crit.pos.y += sinf(ang) * crit.getSpeed();
}

void runBrain(Organism &crit)
{
    if(crit.isDead()) // ded.
        return;

    if(!network::client) // Server-side Logic
    {
        if(random(1,600) == 1 || inputState.key[Key::Space].time == 1)
            crit.brain.lock()->desiredPos = sf::Vector2f(random(10,990),random(10,990));


        if(crit.isHungry())
        {
            Trait* herbivore = nullptr;
            Trait* carnivore = nullptr;
            for(auto &trait : crit.traits)
            {
                if(trait.type == TraitID::Herbivore)
                    herbivore = &trait;
                if(trait.type == TraitID::Carnivore)
                    carnivore = &trait;






            }

            Organism* nearestFood = nullptr;
            float nearestFoodDistance = 99999999;

            if(herbivore != nullptr)
            {
                for(auto &food : crit.sim->flora)
                {
                    if(food.get()->size < herbivore->vars[1]+10) // Vars[1] (The second variable) is how much plant is consumed.
                        continue;


                    float ourDistance = math::distance(crit.pos,food.get()->pos);
                    if(ourDistance < nearestFoodDistance)
                    {
                        nearestFoodDistance = ourDistance;
                        nearestFood = food.get();
                    }
                }
            }

            if(nearestFood != nullptr)
            {
                crit.brain.lock()->desiredPos = nearestFood->pos;
            }
            if(nearestFoodDistance <= crit.size)
            {
                nearestFood->size = std::min(nearestFood->size-herbivore->vars[1],1.f); // Vars[1] (The second variable) is how much plant is consumed.

                crit.nutrition += herbivore->vars[0]; // Vars[0] is how much nutrition is gained from the amount of plant consumed.

            }





        }


    }



    //std::cout << "Critter" + crit.name + ": " + std::to_string(crit.getSpeed()) << std::endl;

    if(math::distance(crit.pos,crit.brain.lock()->desiredPos) < crit.getSpeed()*2)
    {
        crit.pos = crit.brain.lock()->desiredPos;
    }
    else
        moveVector(crit,crit.brain.lock()->desiredPos);
        //moveAngle(crit,math::angleBetweenVectors(crit.pos,crit.brain.lock()->desiredPos));


}


void runBrains(std::list<std::shared_ptr<Organism>>& organismList)
{
    for(auto &crit : organismList)
        runBrain(*crit.get());
}

void runBody(Organism &crit)
{
    crit.age++;


    if(!crit.isDead())
    {
        crit.runHealth();
        crit.runHunger();
        crit.runHydration();
        crit.runGestation();
    }
    else
    {

    }

}

void runCreatures(std::list<std::shared_ptr<Organism>>& organismList)
{

    for(auto &crit : organismList)
    {
        runBody(*crit.get());

        if(!crit.get()->isDead())
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




