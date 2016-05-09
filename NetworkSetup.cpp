#include "NetworkSetup.h"
#include <iostream>
#include <unistd.h>
#include <sstream>

int NetworkSetup::setUpWAN(string myName) {

  vector <string> resultVector = searchForNetworkHardware();
  string str;

  cout << "I am WAN" << endl;
  cout << myName << endl;
/*
  for (int i=0; i < resultVector.size(); i++) {

    str = getOutputFromConsole("iw " + resultVector[i] + " link");

    if (str.find("Not connected.") != string::npos) {
      cout << resultVector[i] + " is trying to set up a WAN connection" << endl;

      writeToFile(resultVector[i], myName);

      getOutputFromConsole("chmod 600 " + myName);
      getOutputFromConsole("sudo cp " + myName + " /etc/NetworkManager/system-connections/");
      getOutputFromConsole("sudo rm " + myName);
      getOutputFromConsole("sudo /etc/init.d/networking restart");
      usleep(1000000);
      str = getOutputFromConsole("sudo nmcli con up id " + myName + " iface " + resultVector[i]);
      if ( str.find("Error") == string::npos) {
        cout << "Setting up the connection was succesful." << endl;
        currentWan = resultVector[i];
        return 0;
      };
    };
  };
  cout << str << endl;
*/
  return 1;
};

int NetworkSetup::connectToWAN(string myNetworkName) {

  vector <string> resultVector = searchForNetworkHardware();
  string str;

  for (int i=0; i < resultVector.size(); i++) {
    str = getOutputFromConsole("iw " + resultVector[i] + " link");
    if (str.find("Not connected.") != string::npos) {
      cout << resultVector[i] + " is trying to connect to " + myNetworkName << endl;
      str = getOutputFromConsole("sudo nmcli d wifi connect " + myNetworkName + " iface " + resultVector[i]);
      if ( str.find("Error") == string::npos) {
        cout << "Setting up the connection was succesful." << endl;
        currentWan = resultVector[i];
        return 0;
      };
    };
  };
  cout << str << endl;
  return 1;
};

string NetworkSetup::getCurrentWan()
{
    return currentWan;
}


int writeToFile(string networkHardware, string myName) {

  ofstream myfile;
  myfile.open (myName.data());
  myfile << "[connection]\n";
  myfile << "id=" + myName + "\n";
  myfile << "uuid=" + getOutputFromConsole("uuidgen");
  myfile << "type=802-11-wireless\n";
  myfile << "\n";
  myfile << "[802-11-wireless]\n";
  myfile << "ssid=" + myName + "\n";
  myfile << "mode=ap\n";
  myfile << "mac-address=" + getOutputFromConsole("ifconfig " + networkHardware + " | awk '/HWaddr/ {print $5}'");
  myfile << "\n";
  myfile << "[ipv6]\n";
  myfile << "method=auto\n";
  myfile << "\n";
  myfile << "[ipv4]\n";
  myfile << "method=shared";
  myfile.close();
  return 0;
};

vector <string> searchForNetworkHardware() {
  string word;
  vector <string> resultVector;

  string str = getOutputFromConsole("ifconfig | grep wlan | awk '{print $1}'");
  istringstream stm(str);

  while(stm >> word) {
    resultVector.push_back(word);
  };
  return resultVector;
};


string getOutputFromConsole(string cmd) {

  string data;
  FILE * stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
    if (stream) {
      while (!feof(stream))
        if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    };
  return data;
};


