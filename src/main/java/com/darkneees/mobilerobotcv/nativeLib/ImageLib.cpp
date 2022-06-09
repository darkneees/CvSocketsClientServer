#include "com_darkneees_mobilerobotcv_nativeLib_ImageLib.h"
#include <iostream>

#include <stdio.h>
#include <winsock2.h>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

using namespace std;

// Подключение lib для Линковщика

#pragma comment(lib, "ws2_32.lib");

JNIEXPORT jbyteArray JNICALL Java_com_darkneees_mobilerobotcv_nativeLib_ImageLib_helloNative
  (JNIEnv *env, jobject thisObject)
{

    WSADATA wsa; // Структура содержащая данные подключения
    SOCKET s; // Структура сокета
    struct sockaddr_in server; // Информация о соединении
    int recv_size;
    char server_reply[512];

    printf("\nInitialization Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
       printf("Failed. Code Error: %d\n", WSAGetLastError());
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
	}

    printf("Connect server succesed!\n");

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

            jbyte* j_version = (jbyte*)calloc(sizeof(jchar), data);
            for(int i=0; i <= data; i++){
                j_version[i] =  (jbyte) frame_data[i];
            }

            jbyteArray j_version_array = env->NewByteArray(data);
            env->SetByteArrayRegion(j_version_array, 0, data , j_version);

            jclass cls_awesome_lib = env -> GetObjectClass(thisObject);
            jmethodID mid_compare = env->GetMethodID(
                    cls_awesome_lib,
                    "setImageCameraView",
                    "([B)V"
                );

            env->CallVoidMethod(
                    thisObject,
                    mid_compare,
                    j_version_array
                );
        }
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
