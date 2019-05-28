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

struct client {
    sockaddr_in sockAddr;
    int socket;
}

int main()
{
    int listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    fcntl(listener, F_SETFL, O_NONBLOCK);
    
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    struct client client;
    vector<struct client> clientvector;
    unsigned int sizeclient = sizeof(clientvector);

    int SIZE_BUF = 1024;
    int bytesSend;
    int bytesRecv = 0;
    char recvbuf[SIZE_BUF] = "";
    char sendbuf[SIZE_BUF] = "";

    int socket = -1;

    while(1)
    {
        while (socket == -1) {
            fd_set rfds;
            FD_ZERO(&rfds);
            FD_SET(listener, &rfds);
            struct timeval tv;
            tv.tv_usec = 0.0;

            int selectRecv = select(listener+1, &rfds, NULL, NULL, &tv);

            if (selectRecv < 0) {
                perror("selectsocket");
                exit(3);
            }
            else if (selectRecv > 0) {
                socket = accept (listener, (sockaddr *) &client, &sizeclient);
                if (strstr(recvbuf, "Test")) {
                    printf(recvbuf);
                    return 0;
                }
                else {
                    if (socket != -1) {
                        bytesRecv = recv(socket, recvbuf, SIZE_BUF, 0);
                        clientvector.push_back(client);
                        cout << "New connecct " + string(inet_ntoa(client.sockAddr.sin_addr)) << endl;
                        if (clientvector.size() > 1) {
                            for (int i = 0; i < clientvector.size() - 2; i++) {
                                bytesSend = send(clientvector[i].socket, "We have new connect", SIZE_BUF, 0);
                                bytesSend = send(clientvector[i].socket, (sockaddr *) &client.sockAddr.sin_addr.s_addr, SIZE_BUF, 0);
                                bytesSend = send(client.socket, (sockaddr *) &clientvector[i].sockAddr.sin_addr.s_addr, SIZE_BUF, 0);
                            }
                        }
                        else {
                            bytesSend = send(clientvector[0].socket, "First connect to server", SIZE_BUF, 0);
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