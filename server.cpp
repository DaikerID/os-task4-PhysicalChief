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
const int SIZE_BUF = 2048;

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
    string password="";
    int logged = 1;
    time_t seconds = time(NULL);
};

int alreadyConnected(clientData& client, vector<clientData>& clients){
    for(auto c : clients){
        if((c.Data.sin_addr.s_addr == client.Data.sin_addr.s_addr)&&(c.password!="")){
            return c.logged;
        }
    }
    return 0;
}

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

            for(auto &c : clients){
                if((time(NULL)-c.seconds>120)&&(c.logged==-1)){
                    c.logged =2;
                    bytesSent = send(socket, "Your IP unblocked! You can login now", SIZE_BUF, 0);
                    cout << endl << "[INFO] IP " << string(inet_ntoa(c.Data.sin_addr))<<" unblocked! You can login now!" << endl;
                }
            }
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
                        if (alreadyConnected(client, clients) == 0) {
                            cout << "[INFO] It seems that we have new user!" << endl;
                            bytesSent = send(socket, "It seems that we have new user!", SIZE_BUF, 0);
                            bytesSent = send(socket, "Please, enter password to sign up!", SIZE_BUF, 0);
                            bytesRecv = recv(socket, recvbuf, SIZE_BUF, 0);
                            if (bytesRecv > 0) {
                                clients.push_back(client);
                                clients[clients.size() - 1].password = recvbuf;
                                clients[clients.size() - 1].logged = 2;

                                bytesSent = send(socket, "Password created! Welcome to server, client!", SIZE_BUF, 0);
                            }
                        } else if (alreadyConnected(client, clients) == 2) {
                            cout << "[INFO] Welcome back, client!" << endl;
                            bytesSent = send(socket, "Welcome back, client!", SIZE_BUF, 0);
                            int n = 3;
                            for (auto &c: clients) {
                                if (c.Data.sin_addr.s_addr == client.Data.sin_addr.s_addr) {
                                    while (n > 0) {
                                        memset(recvbuf, 0, SIZE_BUF * sizeof(char));
                                        bytesSent = send(socket, "Please, enter password to sign up!", SIZE_BUF, 0);
                                        bytesRecv = recv(socket, recvbuf, SIZE_BUF, 0);
                                        if (c.password == recvbuf) {
                                            c.logged = 2;
                                            bytesSent = send(socket, "Password is correct! Welcome to server, client!",
                                                             SIZE_BUF, 0);
                                            break;
                                        } else {
                                            n--;
                                            if (n == 0) {
                                                bytesSent = send(socket,
                                                                 "You have spent all attempts. Your IP will be blocked for 2 minutes.",
                                                                 SIZE_BUF, 0);
                                                c.logged = -1;
                                                c.seconds = time(NULL);
                                                break;
                                            } else {
                                                bytesSent = send(socket, "Password is incorrect! Please try again!",
                                                                 SIZE_BUF, 0);
                                            }
                                        }
                                    }
                                    break;
                                }
                            }
                        } else {
                            bytesSent = send(socket, "Sorry, but your IP blocked for 2 minutes. Please try again later",
                                             SIZE_BUF, 0);
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
