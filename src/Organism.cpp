#include "Organism.h"

std::list<Brain> brainStorage;
std::list<Organism> flora;
std::list<Organism> organisms;

Brain::Brain()
{
    desiresMate = random(0,1);
}




Organism::Organism()
{
    size = random(1,100) * 0.1;
    baseSpeed = random(1,10);

    //* brain = nullptr;
    colorPrime = sf::Color(random(0,255),random(0,255),random(0,255));
    colorSecondary = sf::Color(random(0,255),random(0,255),random(0,255));

    health = getHealthMax();
    nutritionMax = random(50,100);
    hydrationMax = random(50,200);

    nutrition = getNutritionMax();
    hydration = getHydrationMax();
}

float Organism::getHungerRate()
{
    float hungerRate = 0.01;

    hungerRate += 0.01*size;
    //hungerRate += 0.01*strength;
    hungerRate += 0.01*baseSpeed;


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
    if(nutrition < 0)
    {
        nutrition = 0;
        health--;
    }
}

void Organism::runHydration()
{
    hydration -= getThirstRate();
    if(hydration < 0)
    {
        hydration = 0;
        health--;
    }
}

void Organism::runGestation()
{
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
        //runBrain(*crit.get());
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

