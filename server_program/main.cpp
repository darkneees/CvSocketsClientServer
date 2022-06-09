// Подклюение библиотек

#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <string>

#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds


using namespace cv;
// Подключение lib для Линковщика

#pragma comment(lib, "ws2_32.lib");

void detectAndDisplay(Mat frame);
CascadeClassifier body_cascade;


int main(int argc, char *argv[])
{

    String body_cascade_name = samples::findFile("D:\\User\\Desktop\\OpenCV_Diplom\\server_program\\haarcascade_frontalface_alt.xml");

    // Загрузка каскадов

    if(!body_cascade.load(body_cascade_name)) {
        printf("[Server] Error loading file cascade\n");
        return -1;
    }
    printf("Cascade: %s", body_cascade);

    VideoCapture cap(0);
    if(!cap.isOpened()) {
        printf("ERROR: Could not open camera");
        return 1;
    }

    Mat frame;

    WSADATA wsa; // Структура содержащая данные подключения
    SOCKET s, new_socket; // Структура сокета
    struct sockaddr_in server, client; // Информация о соединении
    int c; // Размер структуры
    int recv_size;

    printf("\nInitialization Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
       printf("Failed. Error code: %d\n", WSAGetLastError()); 
       return 1;
    }

    printf("Success!\n");

    // Создание сокета
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Dont create socket. Code error: : %d\n", WSAGetLastError());
    }

    // Функция сокета возвращает дескрепитор сокета на основе введёных данных
    // Для использования сетевых команд
    /*
    Address Family : AF_INET (IPV4 версии)
    Type : SOCK_STREAM (означает что соединение ориетировано на TCP протокол)
    Protocol : 0 [IPPROTO_TCP - 0, IPPROTO_UDP - 1]
*/

    printf("Object socket created\n");

    // Заполним информацию о соединении
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
    server.sin_port = htons(8888);

    /*
    Мы привязываем сокет к определенному IP-адресу и определенному номеру порта. Делая это, 
    мы гарантируем, что все входящие данные, направленные на этот номер порта, 
    будут получены этим приложением.
    */

   if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {

       printf("Bind socket error. Error code: %d\n", WSAGetLastError());
    }

    printf("Bind server success!\n");

    // Перевод сокета в режим прослушивания
    listen(s, 3);

    printf("Expect answer...\n");
    c = sizeof(struct sockaddr_in);

    // При помощи функции accept мы ожидаем подключение клиента

    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET ) {

        printf("Connected client\n");

        startWindowThread();
        while(true) {
            cap.read(frame);
            
            detectAndDisplay(frame);

            frame = (frame.reshape(0,1));
            int imgSize = frame.total()*frame.elemSize();

            int i = send(new_socket, (char*)&imgSize, sizeof(int), 0);
            printf("File size: %d\n", imgSize);
            printf("Expect answer....\n");

            char *message;
            if(recv_size = recv(new_socket , (char *)&message , sizeof(int) , 0) == SOCKET_ERROR)
            {
                printf("Error get data\n");
                break;
            }

            if(int i = send(new_socket, (const char*)frame.data, imgSize, 0) == SOCKET_ERROR) {
                printf("Error get data %d\n", WSAGetLastError());
                break;
            }
            //printf("Picture sended: %s\n", frame.data);
            // std::this_thread::sleep_for (std::chrono::microseconds(30));
        }
        
    }
    

    getchar();

    closesocket(s);
    WSACleanup();
    cap.release();

    return 0;

}


void detectAndDisplay(Mat frame) {

    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    equalizeHist(gray, gray);

    // Detect body
    std::vector<Rect> bodies;
    body_cascade.detectMultiScale(gray, bodies, 1.4, 3, CASCADE_SCALE_IMAGE, Size(30, 30));

    printf("Bodies Size: %d", bodies.size());

    for(size_t i = 0; i < bodies.size(); i++){
        Point center( bodies[i].x + bodies[i].width/2, bodies[i].y + bodies[i].height/2 );
        ellipse(frame, center, Size( bodies[i].width/2, bodies[i].height/2 ), 0, 0, 360, Scalar( 0, 0, 255 ), 4);
        double distance = bodies[i].height / 24 * 25.6;

        printf("Distance: %lf\n", distance);
    }
}