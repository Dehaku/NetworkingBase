#include "Networking.h"
#include "globalvars.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>

/*

TODO: Test with alot of players joining and leaving at the same time to see if threaded clients need to be mutex locked.
TODO: Remove the server's handling of packets mutex, and do a bool instead(with a mutex afterwards)
It's okay for the server to not handle every single packet instantly.
    Or maybe this isn't true since we're using a seperate packet container?
    It may never get the chance to access it, simply always being filled.
    Perhaps it's the Packet RECEIVER, the selector, that should have the waiting bool, and simply
    let the server handle packets it has qued up.

TODO: Add proper client-side connection shutdown

*/

Profile::Profile()
{
    name = "Guest";
    credits = 0;
    pix = 0;
    enteredStringAimer = 0;
}
Profile myProfile;


namespace network
{
    int mainPort = 23636;
    bool packetDeletion = false;
    bool servWait = false;
    bool cliWait = false;
    bool server = false;
    bool client = false;
    bool chatting = false;
    bool needTime = false;
    bool givingTime = false;
    bool listening = false;
    std::string name = "";
    std::string connectedServer = "";
    sf::Mutex clientHandling;
    sf::Mutex packetManagerHandling;
    unsigned int bytesCollected = 0;
    unsigned int megabytesCollected = 0;
    unsigned int gigabytesCollected = 0;
    unsigned int bytesPassed = 0;
    float bytesPerSecond = 0;
}




















