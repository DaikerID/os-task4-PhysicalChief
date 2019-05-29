#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "arpa/inet.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>//для getchar()
#include <stdlib.h>
#include <ctime>
#include <iomanip>
#include <unistd.h>

using namespace std;
const int SIZE_BUF = 2048;

void exitClient(char *message){
    perror(message);
    cout<<"[ERROR] Error type of "<<message<<endl;
    exit(1);
}

int main() {
    int sock;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        exitClient("Socket");
    }
    struct sockaddr_in clientTest;
    // Bind the socket.
    clientTest.sin_family = AF_INET;
    clientTest.sin_addr.s_addr = inet_addr("127.0.0.1");//INADDR_ANY позволяет подключиться машине с любым IP
    clientTest.sin_port = htons(27015);
    while(connect(sock, (struct sockaddr *)&clientTest, sizeof(clientTest)) < 0){
        if(connect(sock, (struct sockaddr *)&clientTest, sizeof(clientTest)) < 0){

        }
        else{
            break;
        }
    }
    socklen_t len = sizeof(clientTest);
    int bytesSent;
    int bytesRecv;
    char *sendbuf;
    char recvbuf[SIZE_BUF];
    bool connect = false;
    while (1) {
        if(!connect) {
            connect=true;
            bytesSent = send(sock, "Test", 7, 0);//Отправляю его на сервер
            if(bytesSent>0){
                cout<<"[INFO] Request for connection sent"<<endl;
                return 0;
            }
            else{
                exitClient("Request");
            }
        }
    }

    system("pause");
    return 0;
}
