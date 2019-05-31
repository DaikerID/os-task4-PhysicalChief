#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
#include <vector>
#include <string.h>
#include <cstring>
#include <iostream>

using namespace std;
const int SIZE_BUF = 32;

void socket_initialize(int &listener, struct sockaddr_in &addr, char * port){
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0){
        cout << "[ERROR] Socket creation error!" << endl;
        perror("socket");
        exit(1);
    }

    // Bind the socket.
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY позволяет подключиться машине с любым IP
    addr.sin_port = htons(atoi(port));

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        cout << "[ERROR] Binding error!" << endl;
        perror("bind");
        exit(2);
    }

    listen(listener, 1);
    cout << "[INFO] Waiting for clients..." << endl;
}

struct clientData{
    clientData(){ ; };
    sockaddr_in Data;
    int socket;
};

int main(int argc, char * argv[])
{
    cout << argc<< endl;
    if(argc!=2){
        cout<<"[ERROR] Invalid number of arguments"<<endl;
        return 1;
    }
    long int port = atoi(argv[1]);
    if(port==0){
        cout<<"[ERROR] Invalid port"<<endl;
        return 1;
    }
    int listener;
    struct sockaddr_in addr;
    socket_initialize(listener, addr, argv[1]);

    // Accept connections.
    clientData client;
    vector <clientData> clients;
    unsigned int len = sizeof(client);

    // Send and receive data.
    int bytesSent;
    int bytesRecv = 0;
    char sendbuf[SIZE_BUF] = "";
    char recvbuf[SIZE_BUF] = "";

    int socket=-1;

    while (1) {
        while (socket == -1) {
            struct timeval tv;
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(listener, &rfds);
            tv.tv_usec = 0.0;
            int selectRecv = select(listener+1,&rfds,NULL,NULL,&tv);

            if (selectRecv<0){
                cout << "[ERROR] Problem with socket!" << endl;
                perror("selectsocket");
                exit(3);
            }
            else if (selectRecv>0) {
                cout << endl << "[INFO] New client can connect" << endl;
                socket = accept(listener, (sockaddr *) &client, &len);
                bytesRecv = recv(socket, recvbuf, SIZE_BUF, 0);
                if(strstr(recvbuf,"Test")){
                    printf(recvbuf);
                    return 0;
                }
                else {
                    memset(recvbuf, 0, SIZE_BUF * sizeof(char));
                    if (socket != -1) {
                        //Записываю IP клиента
                        cout << "[INFO] Connection from " + string(inet_ntoa(client.Data.sin_addr)) << endl;
                        clients.push_back(client);
                        if (clients.size() == 1) {
                            bytesSent = send(socket, "You are first client", SIZE_BUF, 0);
                        }
                        else {
                            for (int i = 0; i < clients.size()-2; i++) {
                                bytesSent = send(clients[i].socket, inet_ntoa(client.Data.sin_addr), SIZE_BUF, 0);
                                bytesSent = send(client.socket, inet_ntoa(clients[i].Data.sin_addr), SIZE_BUF, 0);
                            }
                        }
                    }
                }
            }
            socket = -1;
        }
    }

    system("pause");
    return 0;
}
