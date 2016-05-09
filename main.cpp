#include "network.h"
#include <iostream>

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    RequestServer server;
    Pose pose;
    char* buffer;

//    netSetup.connectToWAN("robotino");

    server.SetName("Robotino1");
    server.connectServer();

    for(int i=0;i<20;i++)
    {
        server.monitorRequest(10000000);
    }

//    client.setCurrentWan(netSetup.getCurrentWan());

//    client.updateNetworkData();
/*
    buffer = client.getRequestName("Alex",GET_POSE,sizeof(Pose));
    if(buffer!=NULL)
    {
        memcpy(&pose,buffer,sizeof(Pose));
        cout<<pose.x<<"  "<<pose.y<<"  "<<pose.theta<<endl;
    }
    double vx = 0.345;

    client.sendRequestName("Alex",COMMAND_MOVE,(char*)0,0);
*/
    //return a.exec();
}
