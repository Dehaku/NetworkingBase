#include "Organism.h"

std::list<brain> BrainStorage;
std::list<organism> Flora;
std::list<organism> Organisms;

class brain
{
public:
    organism *owner;
    sf::Vector2f desiredPos;
    bool desiresMate;
    brain()
    {
        desiresMate = random(0,1);
    }
};

class organism
{
public:
    sf::Vector2f pos;
    float health;
    float baseSpeed;
    float size;
    float nutrition;
    float hydration;

    // Identity
    std::string name;
    brain *brain;
    sf::Color colorPrime;
    sf::Color colorSecondary;


    float ageMax;
    float age;
    float gestationPeriod;
    float gestationTime;


    organism()
    {
        size = random(1,100) * 0.1;
        baseSpeed = random(1,10);

        brain = nullptr;
        colorPrime = sf::Color(random(0,255),random(0,255),random(0,255));
        colorSecondary = sf::Color(random(0,255),random(0,255),random(0,255));

        health = getHealthMax();
        nutrition = getNutritionMax();
        hydration = getHealthMax();
    }

    float getHealthMax()
    {
        return std::max(size * 2,1.f);
    }

    float getSpeed()
    {
        return baseSpeed / size;
    }

    float getNutritionMax()
    {
        return std::max(size / 2,1.f);
    }

    float getHydrationMax()
    {
        return std::max(size,1.f);
    }



};

void moveAngle(organism &crit, float ang)
{
    crit.pos.x += cosf(ang) * crit.getSpeed(); // * Delta? * Gamespeed()! (Delta+TimeWarp)
    crit.pos.y += sinf(ang) * crit.getSpeed();
    //crit.xpos += moveX;
    //crit.ypos += moveY;
}

void runBrain(organism &crit)
{

    if(random(1,600) == 1 || inputState.key[Key::Space].time == 1)
        crit.brain->desiredPos = sf::Vector2f(random(10,990),random(10,990));


    //std::cout << "Critter" + crit.name + ": " + std::to_string(crit.getSpeed()) << std::endl;
    moveAngle(crit,math::angleBetweenVectors(crit.pos,crit.brain->desiredPos));

}

void runBrains()
{
    for(auto &crit : Organisms)
        runBrain(crit);
}

void worldPopulationSetup()
{
    for(int i = 0; i != 100; i++)
    {
        organism Plant;
        Plant.size = random(5,30);
        Plant.baseSpeed = 0;
        Plant.pos = sf::Vector2f(random(10,1000),random(10,1000));
        Plant.colorPrime = sf::Color(0,200,0,150);
        Plant.colorSecondary = sf::Color(0,100,0,150);
        Flora.push_back(Plant);
    }

    for(int i = 0; i != 100; i++)
    {
        organism Critter;
        Critter.name = std::to_string(i); // Temporary
        Critter.pos = sf::Vector2f(random(10,1000),random(10,1000));
        Organisms.push_back(Critter);
        brain creatureBrain;
        BrainStorage.push_back(creatureBrain);

        Organisms.back().brain = &BrainStorage.back();
        BrainStorage.back().owner = &Organisms.back();
    }
}

void addCreatures(int amount)
{
    for(int i = 0; i != amount; i++)
    {
        organism Critter;
        Critter.name = std::to_string(i); // Temporary
        Critter.pos = sf::Vector2f(random(10,1000),random(10,1000));
        Organisms.push_back(Critter);
        brain creatureBrain;
        BrainStorage.push_back(creatureBrain);

        Organisms.back().brain = &BrainStorage.back();
        BrainStorage.back().owner = &Organisms.back();
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
    fpsKeeper.calcFPS();
    //fpsKeeper.framesPerSecond;

    int yOffset = 3;

    sfe::RichText text(gvars::defaultFont);

    text.setFont(gvars::defaultFont);
    text.setPosition(100,100);
    text << sf::Text::Bold       << sf::Color::Cyan  << "This "
         << sf::Text::Italic     << sf::Color::White << "is\ncool\n"
         << sf::Text::Regular    << sf::Color::Green << "mate"
         << sf::Color::White     << ".\n"
         << sf::Text::Underlined << "I wish I could lick it!";

    text.setCharacterSize(25);
    //text.setOrigin(text.getGlobalBounds().width / 2.f, text.getGlobalBounds().height / 2.f);

    window.draw(text);



    //shapes.createText(200,200,15,sf::Color::White, "Hello World! \n Goodbye World!");
    if(inputState.key[Key::Space])
    {
        sfe::RichText text(gvars::defaultFont);
        text.setPosition(200,200);
        text.setCharacterSize(15);
        text << sf::Color::White << "Hello World! \n"
        << sf::Text::Underlined << sf::Color::Red << "Goodbye World!";
        shapes.createRichText(text);
    }
        //shapes.shapes.back().isRichText = true;


    shapes.createText(5,10*1,10,sf::Color::White, "FPS/UPS:" + std::to_string(int(fpsKeeper.framesPerSecond))
                      + "/" + std::to_string(int(fpsKeeper.updatesPerSecond))
                      );
    shapes.createText(5,10*2,10,sf::Color::White, "World Pop:" + std::to_string(Organisms.size()));
    //for(int i = 0; i != Organisms.size(); i++)
    int limitCounter = 0;
    for(auto &crit : Organisms)
    {
        if(limitCounter > 10)
            break;
        std::string descString = "Critter; (" + std::to_string(int(crit.health)) +
            "), " + std::to_string(int(crit.nutrition)) + ":" + std::to_string(int(crit.hydration));

        shapes.createText(5,10*yOffset,10,sf::Color::Cyan, descString);
        yOffset++;
        limitCounter++;
    }


}

void drawCritters()
{
    static bool draw = true;
    static bool drawSquareInstead = false;
    static sf::Color background(150,150,10);



    if(inputState.key[Key::Tab].time == 1)
        toggle(draw);
    if(inputState.key[Key::R].time == 1)
        toggle(drawSquareInstead);

    if(!draw)
        return;

    shapes.createSquare(0,0,1000,1000,background);

    for(auto &plant : Flora)
    {

        shapes.createCircle(plant.pos.x,plant.pos.y,plant.size,plant.colorPrime,plant.size/10,plant.colorSecondary);
    }
    for(auto &crit : Organisms)
    {
        if(drawSquareInstead)
            shapes.createSquare(crit.pos.x-((crit.size)),crit.pos.y-((crit.size)),crit.pos.x+((crit.size)),crit.pos.y+((crit.size)),crit.colorPrime,crit.size/2,crit.colorSecondary);
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

