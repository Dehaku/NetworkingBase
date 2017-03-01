#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "filesystemUtils.hpp"
#include "util.h"

#include "Networking.h"
#include "Text.h"
#include "Simulation.h"

void saveProfile(std::string profileName);
void loadProfile(std::string profileName);

class AddressContainer
{
public:
    std::string address;
    std::string port;
};

void saveConnectAddress(std::string enteredAddress, std::string enteredPort);
AddressContainer loadConnectAddress();

void saveCreatureBlueprint(Organism critter);
Organism loadCreatureBlueprint(std::string blueprintName);

// void saveGame(std::string profileName);
// void loadGame(std::string profileName);

#endif // SAVELOAD_H_INCLUDED
