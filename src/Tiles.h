#ifndef TILES_H_INCLUDED
#define TILES_H_INCLUDED

class worldtile
{
public:
    //sf::Color color;

    bool isWater;
    float moveSpeedModifier;
    float temperature;

    worldtile()
    {
        isWater = false;
        moveSpeedModifier = 0;
        temperature = 50;
    }
};


#endif // TILES_H_INCLUDED
