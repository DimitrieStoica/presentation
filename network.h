#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <poll.h>
#include <sstream>
#include <unistd.h>
#include "NetworkSetup.h"
#include <stdlib.h>
#include <string>

using namespace std;

#define NOT_AVAILABLE 0
#define START         1
#define GET_NAME      2
#define GET_POSE      3
#define GET_MAP       4
#define COMMAND_MOVE  5
#define SET_VX        6
#define SUCCESS       7
#define GET_TASK      8
#define GET_LIST      9

struct Pose {
  double x,y,theta;
};

struct ScanNodeInfo {
  char ip[16];
  char mac[18];
};

class SocketServer {
  int socketServer,socketClient;
  int port;

  public:

  SocketServer(int port) {
    this->port = port;
  };

  SocketServer() {
    this->port = 8080;
  };

  int connectServer();
  int monitorSocket(int time);
  int readData(char* buffer,int dataSize);
  int writeData(char *buffer,int dataSize);
  void closeSocketClient();
  void closeSocketServer();
};



class SocketClient {
  int socketClient;

  public:

  SocketClient()
  {
      socketClient = 0;
  }

  int connectClient(char* ip, int port);
  int readData(char* buffer, int dataSize);
  int writeData(char *buffer,int dataSize);
  void closeSocket();
};

class RequestClient {
  SocketClient client;
  vector<ScanNodeInfo> currentNetworkInfo;
  int port;
  string currentWan;
  Pose pose;
  double vx;

  public:

  RequestClient()
  {
      port = 8080;
      vx = 0.345;
  };

  char* getRequestIp(char* ip,int getType, int dataSize);
  char* getRequestName(char* name,int getType, int dataSize);
  int sendRequestName(char* name,int getType, char* buffer,int dataSize);
  void SetPort(int port);
  vector<ScanNodeInfo> scanNetworkWAN();
  int updateNetworkData();
  void setCurrentWan(string wan);

};

class RequestServer {
  SocketServer srv;
  NetworkSetup netSetup;
  RequestClient client;

  Pose pose;
  char name[9];

  public:

  RequestServer() {
    pose.x = 241;
    pose.y = 12;
    pose.theta = 124;
  };

  void connectServer();
  int monitorRequest(int time);
  void SetName(char *name);
};

#endif // NETWORK_H
