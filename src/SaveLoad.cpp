
#include "SaveLoad.h"

void saveProfile(std::string profileName)
{

    std::string line("data/saves/" + profileName);

    galaxy_mkdir(line);
    line.append("/save");
    line.append(".profile");

    { // General/Misc data.
        /*
        std::string miscLine("data/saves/" + profileName);
        miscLine.append("/save");
        miscLine.append(".misc");
        std::ofstream outputFileMisc(miscLine.c_str());

        outputFileMisc << "[Cash:" << conFact->credits << "]"
        << "[GameBeaten:" << bountytower::gameBeaten << "]";
        */
    }



    std::ofstream outputFile(line.c_str());
    outputFile << "[name:" << myProfile.name << "]"
               << "[credits:" << myProfile.credits << "]"
               << "[pix:" << myProfile.pix << "]";
    chatBox.addChat("Profile Saved!");
}

void loadProfile(std::string profileName)
{
    std::cout << "Loading Game Profile: " + profileName + "!\n";

    { // Loading generic/misc data.
        std::ifstream input("data/saves/" + profileName + "/save.profile");
        if (!input.is_open())
        {
            std::cout << "No Profile Detected. \n";
            Profile newProfile;
            myProfile = newProfile;
            return;
        }


        while (input.good())
        {
            std::string line;
            getline(input, line);

            // Load the variables!
            myProfile.name = stringFindString(line,"name:");
            myProfile.credits = stringFindNumber(line,"credits:");
            myProfile.pix = stringFindNumber(line,"pix:");
        }
    }



    /*

    { // Loading generic/misc data.
        std::ifstream input("data/saves/" + profileName + "/save.misc");
        if (!input.is_open())
            return;

        con("Misc file is open.");

        while (input.good())
        {
            std::string line;
            getline(input, line);
            conFact->credits = stringFindNumber(line,"Cash:");
            bountytower::gameBeaten = booleanize(stringFindNumber(line, "[GameBeaten:"));
        }
    }

    for(auto squaddie : Squaddies)
        squaddie->toDelete = true;
    Squaddies.clear();

    std::ifstream input("data/saves/" + profileName + "/save.crit");
    if (!input.is_open())
        return;

    con("Critter File is open.");

    while (input.good())
    {
        std::string line;
        getline(input, line);
        Npc critter;

        critter.name = "Debuggery";
        std::string critterName = stringFindString(line, "[name:");
        if(critterName == "")
        { // This is an invalid line, Next!
            std::cout << "This line doesn't have a name! Invalid. \n";
            continue;
        }
        critter.race = stringFindString(line, "[race:");

        // This should save some manual work.
        critter = *getGlobalCritter(critter.race);
        critter.name = critterName;
        critter.skillpoints = stringFindNumber(line, "[skillpoints:");
        critter.xpos = stringFindNumber(line, "[xpos:");
        critter.ypos = stringFindNumber(line, "[ypos:");
        critter.zpos = stringFindNumber(line, "[zpos:");
        critter.level = stringFindNumber(line, "[level:");
        critter.xp = stringFindNumber(line, "[xp:");
        critter.additionalSlots = stringFindNumber(line, "[slots:");
        critter.attributes.strength = stringFindNumber(line, "[strength:");
        critter.attributes.perception = stringFindNumber(line, "[perception:");
        critter.attributes.intelligence = stringFindNumber(line, "[intelligence:");
        critter.attributes.charisma = stringFindNumber(line, "[charisma:");
        critter.attributes.endurance = stringFindNumber(line, "[endurance:");
        critter.attributes.dexterity = stringFindNumber(line, "[dexterity:");
        critter.cbaseid = stringFindNumber(line, "[cbaseid:");
        critter.tags = stringFindChaos(line,"{Tags:","}");
        std::cout << "Tags: " << critter.tags << std::endl;
        //status.auraAffectsAllies = booleanize(stringFindNumber(line, "[AuraAllies:"));

        con("Loading Skills");
        std::vector<std::string> skills = stringFindVectorChaos(line,"[Skill:","]");
        for(auto &skillLine : skills)
        {
            std::vector<std::string> skillBits = stringFindElements(skillLine);
            for(auto &skill : critter.skills.list)
                if(skillBits[0] == skill.name)
                    skill.ranks = std::stoi(skillBits[1]);
        }

        critter.health = critter.getMaxHealth();
        critter.isSquaddie = true;
        critter.faction = conFact->name;
        critter.factionPtr = conFact;
        critter.chasePriority = "Hold Position";
        std::cout << "Loaded " << critter.name << " the " << critter.race << std::endl;
        npclist.push_back(critter);
    }

    AnyDeletes(npclist);
    Squaddies.clear();
    for(auto &npc : npclist)
        if(npc.faction == "The Titanium Grip" && npc.isSquaddie)
            Squaddies.push_back(&npc);

    std::ifstream itemInput("data/saves/" + profileName + "/save.inv");
    if (!itemInput.is_open())
        return;

    Item * lastItem;
    while (itemInput.good())
    {
        std::string line;
        getline(itemInput, line);

        Item item;

        item.name = "Debuggery";
        std::string itemName = stringFindString(line, "[name:");
        if(itemName == "")
        { // This is an invalid line, Next!
            std::cout << "This line doesn't have a name! Invalid. \n";
            continue;
        }
        //con("Getting item: " + itemName);
        //item = *getGlobalItem(itemName);

        item = makeItemFromString(line);

        //Clearing status effects so that loading a preexisting item doesn't dupe the statuses.
        item.statusEffects.clear();
        item.statusEffectsInflict.clear();
        item.statusEffectsCarried.clear();

        item.amount = stringFindNumber(line, "[amount:");

        std::vector<std::string> statusEffects = stringFindVectorChaos(line,"{StatusEffect:","}");
        std::vector<std::string> statusEffectInflicts = stringFindVectorChaos(line,"{StatusEffectInflict:","}");
        std::vector<std::string> statusEffectCarried = stringFindVectorChaos(line,"{StatusEffectCarried:","}");

        con("Status Effects");
        for(auto &statusString : statusEffects)
        {
            StatusEffect status;
            status.name = stringFindString(statusString, "[Name:");
            status.rank = stringFindString(statusString, "[Rank:");
            status.duration = stringFindNumber(statusString, "[Duration:");
            status.auraAffectsAllies = booleanize(stringFindNumber(statusString, "[AuraAllies:"));
            status.auraAffectsEnemies = booleanize(stringFindNumber(statusString, "[AuraEnemies:"));
            status.auraAffectsNeutrals = booleanize(stringFindNumber(statusString, "[AuraNeutrals:"));

            std::vector<std::string> aspectStrings = stringFindVectorChaos(statusString, "[Aspect:","]");
            for(auto &aspectString : aspectStrings)
            {
                StatusAspect aspect;

                std::vector<std::string> aspectBits = stringFindElements(aspectString);
                for(int i = 0; i != 3; i++)
                {
                    if(i == 0)
                        aspect.name = std::stoi(aspectBits[i]);
                    if(i == 1)
                        aspect.potency = std::stoi(aspectBits[i]);
                    if(i == 2)
                        aspect.type = aspectBits[i];
                }
                status.aspects.push_back(aspect);
            }
            con(item.name + " is infused with " + status.name + "!");
            item.statusEffects.push_back(status);
        }

        con("Status Effect Inflicting");
        for(auto &statusString : statusEffectInflicts)
        {
            StatusEffect status;
            status.name = stringFindString(statusString, "[Name:");
            status.rank = stringFindString(statusString, "[Rank:");
            status.duration = stringFindNumber(statusString, "[Duration:");
            status.auraAffectsAllies = booleanize(stringFindNumber(statusString, "[AuraAllies:"));
            status.auraAffectsEnemies = booleanize(stringFindNumber(statusString, "[AuraEnemies:"));
            status.auraAffectsNeutrals = booleanize(stringFindNumber(statusString, "[AuraNeutrals:"));

            std::vector<std::string> aspectStrings = stringFindVectorChaos(statusString, "[Aspect:","]");
            for(auto &aspectString : aspectStrings)
            {
                StatusAspect aspect;

                std::vector<std::string> aspectBits = stringFindElements(aspectString);
                for(int i = 0; i != 3; i++)
                {
                    if(i == 0)
                        aspect.name = std::stoi(aspectBits[i]);
                    if(i == 1)
                        aspect.potency = std::stoi(aspectBits[i]);
                    if(i == 2)
                        aspect.type = aspectBits[i];
                }
                status.aspects.push_back(aspect);
            }
            con(item.name + " is infused with inflicting " + status.name + "!");
            item.statusEffectsInflict.push_back(status);
        }

        con("Status Effect Carried");
        for(auto &statusString : statusEffectCarried)
        {
            StatusEffect status;
            status.name = stringFindString(statusString, "[Name:");
            status.rank = stringFindString(statusString, "[Rank:");
            status.duration = stringFindNumber(statusString, "[Duration:");
            status.auraAffectsAllies = booleanize(stringFindNumber(statusString, "[AuraAllies:"));
            status.auraAffectsEnemies = booleanize(stringFindNumber(statusString, "[AuraEnemies:"));
            status.auraAffectsNeutrals = booleanize(stringFindNumber(statusString, "[AuraNeutrals:"));

            std::vector<std::string> aspectStrings = stringFindVectorChaos(statusString, "[Aspect:","]");
            for(auto &aspectString : aspectStrings)
            {
                StatusAspect aspect;

                std::vector<std::string> aspectBits = stringFindElements(aspectString);
                for(int i = 0; i != 3; i++)
                {
                    if(i == 0)
                        aspect.name = std::stoi(aspectBits[i]);
                    if(i == 1)
                        aspect.potency = std::stoi(aspectBits[i]);
                    if(i == 2)
                        aspect.type = aspectBits[i];
                }
                status.aspects.push_back(aspect);
            }
            con(item.name + " is infused with carrying " + status.name + "!");
            item.statusEffectsCarried.push_back(status);
        }




        std::string itemOwner = stringFindString(line, "[owner:");
        if(itemOwner != "")
        {
            con("Looking for " + itemOwner);
            for(auto &squaddie : Squaddies)
            {
                if(squaddie->name == itemOwner)
                {
                    con(itemOwner + " found, adding " + item.name);
                    squaddie->inventory.push_back(item);
                    lastItem = &squaddie->inventory.back();
                }
            }
        }

        if(line.find("[InternalItem]") != std::string::npos)
        {
            con("Internal Item, Shoving into " + lastItem->name);
            lastItem->internalitems.push_back(item);
        }
    }

    */

    chatBox.addChat("Profile Loaded Successfully!");
    //chatBox.addChat(" Game's loaded, You're good to go recollect some bounties",sf::Color::White);
}

void saveCreatureBlueprint(Organism critter)
{

    // std::string line("data/saves/" + myProfile.name + "/blueprints");
    std::string line("data/saves/Sopheroph/blueprints");

    galaxy_mkdir(line);
    line.append("/Critter" + critter.name);
    line.append(".crit");



    std::ofstream outputFile(line.c_str());

    outputFile
                << "[name:" << critter.name << "]"
                << "[ageMax:" << critter.ageMax << "]"
                << "[baseSpeed:" << critter.baseSpeed << "]"

                << "[gestationPeriod:" << critter.gestationPeriod << "]"
                << "[hydrationMax:" << critter.hydrationMax << "]"
                << "[nutritionMax:" << critter.nutritionMax << "]"
                << "[size:" << critter.size << "]"

                << "[pC.r:" << int(critter.colorPrime.r) << "]"
                << "[pC.g:" << int(critter.colorPrime.g) << "]"
                << "[pC.b:" << int(critter.colorPrime.b) << "]"

                << "[sC.r:" << int(critter.colorSecondary.r) << "]"
                << "[sC.g:" << int(critter.colorSecondary.g) << "]"
                << "[sC.b:" << int(critter.colorSecondary.b) << "]"
                ;

    for(auto &trait : critter.traits)
    {
        outputFile << "[Trait:" << trait.type;

        for(auto &variable : trait.vars)
        {
            outputFile << ":" << variable;
        }
        outputFile << "]";
    }

    chatBox.addChat("Creature Blueprint Saved!");


}

Organism loadCreatureBlueprint(std::string blueprintName)
{


    Organism critter;

     //std::cout << "Loading Game Profile: " + profileName + "!\n";

    std::string line("data/saves/Sopheroph/blueprints");

    { // Loading generic/misc data.
        std::ifstream input("data/saves/Sopheroph/blueprints/Critter" + blueprintName + ".crit");
        if (!input.is_open())
        {
            std::cout << "No Profile Detected. \n";
            chatBox.addChat("Unabled to find creature ''" + blueprintName + "'' ");
            critter.name = "Failed";
            return critter;
        }


        while (input.good())
        {
            std::string line;
            getline(input, line);

            // Load the variables!
            critter.name = stringFindString(line,"name:");
            critter.ageMax = stringFindNumber(line,"ageMax:");
            critter.baseSpeed = stringFindNumber(line,"baseSpeed:");

            critter.gestationPeriod = stringFindNumber(line,"gestationPeriod:");
            critter.hydrationMax = stringFindNumber(line,"hydrationMax:");
            critter.nutritionMax = stringFindNumber(line,"nutritionMax:");
            critter.size = stringFindNumber(line,"size:");

            critter.colorPrime.r = stringFindNumber(line,"pC.r:");
            critter.colorPrime.g = stringFindNumber(line,"pC.g:");
            critter.colorPrime.b = stringFindNumber(line,"pC.b:");

            critter.colorSecondary.r = stringFindNumber(line,"sC.r:");
            critter.colorSecondary.g = stringFindNumber(line,"sC.g:");
            critter.colorSecondary.b = stringFindNumber(line,"sC.b:");



            critter.traits.clear();
            std::vector<std::string> traits = stringFindVectorChaos(line,"[Trait:","]");
            for(auto &traitLine : traits)
            {
                std::vector<std::string> traitBits = stringFindElements(traitLine);
                Trait trait;
                trait.type = std::stoi(traitBits[0]);
                for(int i = 0; i != traitBits.size(); i++)
                {
                    if(i == 0)
                        continue;

                    trait.vars.push_back(std::stof(traitBits[i]));
                }

                critter.traits.push_back(trait);
            }
        }
    }


    chatBox.addChat("Blueprint Loaded Successfully!");

    return critter;
}

void saveConnectAddress(std::string enteredAddress, std::string enteredPort)
{
    std::string line("data/");

    galaxy_mkdir(line);
    line.append("lastAddress");
    line.append(".la");

    std::ofstream outputFile(line.c_str());
    outputFile << "[Address:" << enteredAddress << "]"
    << "[Port:" << enteredPort << "]";

}

AddressContainer loadConnectAddress()
{
    AddressContainer addCon;
    std::ifstream input("data/lastAddress.la");
    while (input.good())
    {
        std::string line;
        getline(input, line);

        // Load the variables!
        addCon.address = stringFindString(line,"Address:");
        addCon.port = stringFindString(line,"Port:");
        std::cout << "Last Address: " << addCon.address << std::endl
            << "Last Port: " << addCon.port << std::endl;
        return addCon;
    }
    addCon.address = "";
    return addCon;
}

