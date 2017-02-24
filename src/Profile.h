#ifndef PROFILE_H_INCLUDED
#define PROFILE_H_INCLUDED

#include <memory>

class Organism;

class GeneralTracker
{
public:
    std::weak_ptr<Organism> selectedOrganism;
};
extern GeneralTracker generalTracker;

#endif // PROFILE_H_INCLUDED
