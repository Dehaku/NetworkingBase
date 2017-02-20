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

void saveProfile(std::string profileName);
void loadProfile(std::string profileName);

// void saveGame(std::string profileName);
// void loadGame(std::string profileName);

#endif // SAVELOAD_H_INCLUDED
