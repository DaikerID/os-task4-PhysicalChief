#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <set>
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

    int sizeClient = 0;
    vector<long> client(n);

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
    int sizeclient = sizeof(clientvector);

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
                        if (clientvector.size() > 1) {
                            for (int i = 0; i < clientvector.size() - 2; i++) {
                                bytesSend = send(clientvector[i].socket, "We have new connect", SIZE_BUF, 0);
                                bytesSend = send(clientvector[i].socket, client.sockAddr.sin_addr.s_addr, SIZE_BUF, 0);
                                bytesSend = send(client, clientvector[i].sockAddr.sin_addr.s_addr, SIZE_BUF, 0);
                            }
                        }
                        else {
                            
                        }
                    }
                }
            }

        }
    }
    
    return 0;
}