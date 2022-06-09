#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>

#include <iostream>

using namespace std;
using namespace cv;

void detectAndDisplay(Mat frame);

CascadeClassifier body_cascade;

int main(int argc, const char** argv) {

    String body_cascade_name = samples::findFile("D:\\User\\Desktop\\OpenCV_Diplom\\server_program\\haarcascade_frontalface_alt.xml");

    // Загрузка каскадов

    if(!body_cascade.load(body_cascade_name)) {
        printf("[Server] Error loading file cascade\n");
        return -1;
    }

    int camera_device = 0;
    VideoCapture capture;
    capture.open(camera_device);

    if(!capture.isOpened()){
        printf("[Server] Error opening video capture\n");
        return -1;
    }

    Mat frame;
    while (capture.read(frame))
    {
        if(frame.empty()) {
            printf("[Server] Error no captured frame\n");
        }

        detectAndDisplay(frame);
        if(waitKey(10) == 27) {
            break;
        }
    }

    return 0;
}

void detectAndDisplay(Mat frame) {

    Mat gray;
    cvtColor(frame, gray, COLOR_BGR2GRAY);
    GaussianBlur(gray, gray, Size (5, 5), 0);
    equalizeHist(gray, gray);

    // Detect body
    std::vector<Rect> bodies;
    body_cascade.detectMultiScale(gray, bodies, 1.4, 3, CASCADE_SCALE_IMAGE, Size(30, 30));

    // printf("Size: %d\n", bodies.size());

    for(size_t i = 0; i < bodies.size(); i++){
        
        printf("Body height: %d\n", bodies[i].height);
        Point center( bodies[i].x + bodies[i].width/2, bodies[i].y + bodies[i].height/2 );
        ellipse( frame, center, Size( bodies[i].width/2, bodies[i].height/2 ), 0, 0, 360, Scalar( 0, 0, 255 ), 4);
        double distance = bodies[i].height / 24 * 25.6;

        printf("Distance: %lf\n", distance);
    }

    imshow("Result", frame);
}