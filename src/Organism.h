#ifndef ORGANISM_H_INCLUDED
#define ORGANISM_H_INCLUDED
#include <list>
#include <algorithm>
#include <iomanip>

#include "Shapes.h"
#include "util.h"

class organism;
class brain;

extern std::list<brain> BrainStorage;
extern std::list<organism> Organisms;
extern std::list<organism> Flora;

void runBrains();

void worldPopulationSetup();

void displayCrittersInfo();
void drawCritters();



#endif // ORGANISM_H_INCLUDED
