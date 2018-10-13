#ifndef FASTESTCAR_DETECTOR_H
#define FASTESTCAR_DETECTOR_H
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#define DIST_MAX 1000000
#define DIST_MIN -1000000
using namespace std;
using namespace cv;


class Detector {
public:
    Detector(int road_horizon);
    int *detect(Mat frame);
private:
    int vote;
    double roi_theta;
    int road_horizon;
    // 所有的底部指y = 0, 视觉上是顶部
    // 计算线与底部的交点和底部中点的距离
    double dist_cross_base(double x1, double y1, double x2, double y2, int width);
};


#endif //FASTESTCAR_DETECTOR_H
