#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include <chrono>
#include <thread>


#include <windows.h>
#include <string>

using namespace cv;
using namespace std;

// Подключение lib для Линковщика

#pragma comment(lib, "ws2_32.lib");

int main(int argc, char *argv[])
{

    int WIDTH = 640;    
    int HEIGHT = 480;



    WSADATA wsa; // Структура содержащая данные подключения
    SOCKET s; // Структура сокета
    struct sockaddr_in server; // Информация о соединении
    int recv_size;
    char server_reply[512];

    printf("\nInitialization Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
       printf("Failed. Code Error: %d\n", WSAGetLastError()); 
       return 1;
    }

      printf("Success!\n");

    // Создание сокета
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Dont create socket. Code Error: %d\n", WSAGetLastError());
    }

    printf("Object socket created!\n");
    server.sin_addr.s_addr = inet_addr("192.168.1.103");
	server.sin_family = AF_INET;
	server.sin_port = htons( 8888 );

    //Connect to remote server
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		printf("Error connect to server");
		return 1;
	}

    printf("Connect server succesed!\n");

    startWindowThread();

    while(true) {
        int data = 0;
        //Receive a reply from the server
        if((recv_size = recv(s , (char *)&data , sizeof(int) , MSG_WAITALL)) == SOCKET_ERROR)
        {
            printf("Error get data\n");
            break;
        }
        if(data != 0) {

            printf("Get Data: %d\n", data);
            printf("Expect answer...\n");

            char* message;
            message = "A";
            send(s, message, strlen(message), 0);

            char *frame_data = new char[data];

            if(recv_size = recv(s, frame_data, data, 0) == SOCKET_ERROR) {
                printf("Error get data\n");
                break;
            }
            printf("Pixels: %s", frame_data);
            

                        
            Mat img(Size(WIDTH, HEIGHT), CV_8UC3, frame_data);
            imshow("Window", img);
            waitKey(30);
            delete []frame_data;
        }

    }

    closesocket(s);
    WSACleanup();

    return 0;
}
