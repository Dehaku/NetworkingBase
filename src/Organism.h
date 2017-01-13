#ifndef ORGANISM_H_INCLUDED
#define ORGANISM_H_INCLUDED
#include <list>
#include <algorithm>
#include <iomanip>

#include "Shapes.h"
#include "util.h"

class organism;

extern std::list<organism> Organisms;

void worldPopulationSetup();

void displayCritters();



#endif // ORGANISM_H_INCLUDED
