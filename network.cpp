#include "network.h"


// Socket Server Class
int SocketServer::connectServer() {
  struct sockaddr_in server;
  int i = i;

  //creates a socket
  socketServer = socket(AF_INET , SOCK_STREAM , 0);

  if (socketServer == -1) {
    perror("Error socket could not be created.");
    return -1;
  };

  if (setsockopt (socketServer, SOL_SOCKET, SO_REUSEADDR, (void* )&i, (socklen_t)(sizeof(int))) < 0) {
    perror("Error setting socket options.");
    return -1;
  };

  cout << "Server socket has been created." << endl;

  //prepares the socket structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(port);

  if (bind(socketServer,(struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("Binding port with socket failed.");
    return -1;
  };

  cout << "Binding port with socket was successful." << endl;

  //it listens for a connection
  listen(socketServer, 3);
  return 0;
};

int SocketServer::monitorSocket(int time) {
  struct pollfd ufds[1];
  struct sockaddr_in client;

  ufds[0].fd = socketServer;
  ufds[0].events = POLLIN;
  int rv = poll(ufds, 1, time);

  if (rv == -1) {
    perror("poll");
    return -1;
  } else if (rv == 0) {
    return 0;
  } else {
    int c = sizeof(struct sockaddr_in);
    socketClient = accept(socketServer, (struct sockaddr *)&client, (socklen_t*)&c);
    return socketClient;
  };
};

int SocketServer::writeData(char* buffer,int dataSize)
{
    int ret;

    ret = write(socketClient, buffer, dataSize) ;
    if (ret < 0)
    {
      cout<<"Error sending data to the socket"<<endl;
      return -1;
    };

    return ret;
}

int SocketServer::readData(char *buffer,int dataSize)
{
    int ret;

    ret = recv(socketClient, buffer, dataSize,0);
    if (ret < 0)
    {
        cout<<"Error reading data from the socket"<<endl;
        return -1;
    }

    return ret;
}

void SocketServer::closeSocketClient()
{
    close(socketClient);
}

void SocketServer::closeSocketServer()
{
    close(socketServer);
}



// Socket Client Class
int SocketClient::connectClient(char* ip, int port) {
  struct sockaddr_in client;
  struct timeval tv;
  int i = i;

  //creates a socket
  socketClient = socket(AF_INET , SOCK_STREAM , 0);

  if (socketClient == -1) {
    perror("Error socket could not be created");
    return -1;
  };

  tv.tv_sec = 1;
  tv.tv_usec = 0;

  if (setsockopt (socketClient, SOL_SOCKET, SO_REUSEADDR, (void* )&i, (socklen_t)(sizeof(int))) < 0) {
    perror("Error setting socket options");
    return -1;
  };
  if (setsockopt (socketClient, SOL_SOCKET,SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval)) < 0) {
    perror("Error setting socket options");
    return -1;
  };
  if (setsockopt (socketClient, SOL_SOCKET,SO_SNDTIMEO, (char*)&tv, sizeof(struct timeval)) < 0) {
    perror("Error setting socket options");
    return -1;
  };

  cout << "Client socket has been created" << endl;

  //prepares the socket structure
  memset(&client,0,sizeof(client));
  client.sin_port = htons(port);
  client.sin_family = AF_INET;
  client.sin_addr.s_addr = inet_addr(ip);

  //connects to the socket
  if (connect(socketClient, (struct sockaddr*)&client, sizeof(client)) < 0) {
    //perror("Error client was not able to connect to server");
    cout<<"Error client was not able to connect to server"<<endl;
    return -1;
  };

  cout << "Client connected to server" << endl;
  return 0;
};

int SocketClient::writeData(char* buffer,int dataSize)
{
    int ret;

    ret = write(socketClient, buffer, dataSize) ;

    if (ret < 0)
    {
      cout<<"Error sending data to the socket"<<endl;
      return -1;
    };

    return ret;
}

int SocketClient::readData(char *buffer,int dataSize)
{
    int ret;

    ret = recv(socketClient, buffer, dataSize, 0);
    if (ret < 0)
    {
        cout<<"Error reading data from the socket"<<endl;
        return -1;
    }

    return ret;
}

void SocketClient::closeSocket()
{
    close(socketClient);
}


// Request Server Class
int RequestServer::monitorRequest(int time) {
  int getType,num,ret;
  char buffer[4];
  string nameWLAN;
  vector <ScanNodeInfo> scanNodeInfoVector;
  string list_data;
  char *S;
  string task = "caca";
  ret = srv.monitorSocket(time);

  if(ret > 0)
  {
      //cout<<ret<<endl;
      ret = srv.readData((char*)buffer,sizeof(int));
      if (ret < 0)
      {
          //perror("Error reading data from the socket");
          return -1;
      };

    istringstream (buffer) >> num;
    cout << num << endl;

    if (num != 0) { 
      std::string getTypeStr(buffer, buffer+ret); // Convert buffer to string.
      getType = std::stoi(getTypeStr);
      cout << getType << endl;
      cout << "i am node" << endl;
    } else {
      memcpy(&getType,buffer,sizeof(int));
      cout << getType << endl;
      cout << "i am c++" << endl; 
    };

    switch (getType) {
    case START:
      ret = SUCCESS;
      if (netSetup.setUpWAN(name) == 1) {
        srv.writeData((char*)&ret,sizeof(int));
      };
      break;
    case GET_LIST:
      client.setCurrentWan(netSetup.getCurrentWan());
      scanNodeInfoVector = client.scanNetworkWAN();
      for (int i=0; i < scanNodeInfoVector.size(); i++){
        list_data = list_data.append(scanNodeInfoVector[i].ip);
        list_data = list_data.append(" ");
      };

      S = new char[list_data.length() + 1];
      strcpy(S,list_data.c_str());

      if (srv.writeData(S,list_data.length()) < 0) {
        perror("Error sending data to the socket");
        return -1;
      };
      break;
    case GET_TASK:
      cout << task << endl;
      S = new char[task.length() + 1];
      strcpy(S,task.c_str());
      if (srv.writeData(S,task.length()) < 0)
      {
        perror("Error sending data to the socket");
        return -1;
      };
      break;
    case GET_NAME:
      if (srv.writeData((char*)name,9) < 0)
      {
          perror("Error sending data to the socket");
          return -1;
      };
      break;
    case GET_POSE:
list_data = "55 241 687";
S = new char[list_data.length() + 1];
strcpy(S,list_data.c_str());

      
if (srv.writeData(S,list_data.length()) < 0) { 
          perror("Error sending data to the socket");
          return -1;
      };
      break;
    case COMMAND_MOVE:
        cout<<"Command move"<<endl;
      break;
    case SET_VX:
        double vx;
        if (srv.readData((char*)&vx,sizeof(double)) < 0)
        {
            perror("Error sending data to the socket");
            return -1;
        };
        cout<<"Vx"<<vx<<endl;
      break;
      default:
        ret = NOT_AVAILABLE;
        if (srv.writeData((char*)&ret,sizeof(int)) < 0)
        {
            perror("Error sending data to the socket");
            return -1;
        };
        break;
     };
     srv.closeSocketClient();
  }
  else
  {
      cout<<"No request"<<endl;
  }
  return 1;
};

void RequestServer::connectServer()
{
    srv.connectServer();
}

void RequestServer::SetName(char *name)
{
    strcpy(this->name,name);
}



// Request client class
char* RequestClient::getRequestIp(char* ip,int getType, int dataSize) {
  char* buffer;

  if(client.connectClient(ip,port)==0)
  {
      client.writeData((char*)&getType,sizeof(int));

      //reads data from the socket
      buffer = new char[dataSize];
      client.readData(buffer,dataSize);

      cout << "Connection terminated\n";
      client.closeSocket();

      return buffer;
  }
  return NULL;
};

char* RequestClient::getRequestName(char* name,int getType, int dataSize)
{
    char* buffer;
    char ip[16];
    int i;

};

int RequestClient::sendRequestName(char* name, int getType, char *buffer, int dataSize)
{
          cout << "Connection terminated\n";
};

void RequestClient::SetPort(int port)
{
    this->port = port;
}

void RequestClient::setCurrentWan(string wan)
{
    currentWan = "eth0";
}

vector<ScanNodeInfo> RequestClient::scanNetworkWAN()
{
    vector <string> nodeIPMACvector;
    vector <ScanNodeInfo> scanNodeInfoVector;
    string word;

    cout<<currentWan<<endl;

    string str = getOutputFromConsole("sudo arp-scan --interface=" + currentWan + " --localnet | awk 'FNR > 2 {print $1 \" \" $2}' | head -n -3");

    cout<<str<<endl;

    if (str.size() > 0)
    {
        istringstream stm(str);
        while(stm >> word)
        {
            nodeIPMACvector.push_back(word);
        };

        for (int j=0; j < nodeIPMACvector.size() - 1; j+=2)
        {
          ScanNodeInfo tmp;
          strcpy(tmp.ip, nodeIPMACvector[j].data());
          strcpy(tmp.mac, nodeIPMACvector[j+1].data());
          scanNodeInfoVector.push_back(tmp);
        }
    }
    return scanNodeInfoVector;
};

int RequestClient::updateNetworkData()
{
    int i;
    int j;

    vector<ScanNodeInfo> scanNetwork = scanNetworkWAN();

    cout << "network size " + scanNetwork.size() << endl;
};




