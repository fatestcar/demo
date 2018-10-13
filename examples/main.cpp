#include <iostream>
#include <cmath>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Detector.h"
#define DIST_MAX 1000000
#define DIST_MIN -1000000
using namespace std;
using namespace cv;


int main() {
    int road_horizon = 240;
    Detector detector(240);
    String video_path = "./video.avi";
    VideoCapture capture(video_path);
    while(capture.isOpened()){
        Mat frame;
        capture.read(frame);
        int* l = detector.detect(frame);
        line(frame, Point(l[0],l[1]), Point(l[2],l[3]), Scalar(0, 0, 255), 3);
        line(frame, Point(l[4],l[5]), Point(l[6],l[7]), Scalar(0, 0, 255), 3);
        imshow("img", frame);
        if(!waitKey(1))
            break;
    }

    return 0;
}
