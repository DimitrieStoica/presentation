#ifndef NETWORKSETUP_H
#define NETWORKSETUP_H
#include <fstream>
#include <string.h>
#include <vector>

using namespace std;

class NetworkSetup
{
    string currentWan;

public:
    int setUpWAN(string myName);
    int connectToWAN(string myNetworkName);
    string getCurrentWan();
};

string getOutputFromConsole(string cmd);
int writeToFile(string networkHardware, string myName);
vector <string> searchForNetworkHardware();

#endif // NETWORKSETUP_H

