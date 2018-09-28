#include <cstdlib>
#include <iostream>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include "GPIOlib.h"


#define PI 3.1415926

//Uncomment this line at run-time to skip GUI rendering
#define _DEBUG

using namespace cv;
using namespace std;
//using namespace GPIO;

const string CAM_PATH="/dev/video0";
const string MAIN_WINDOW_NAME="Processed Image";
const string CANNY_WINDOW_NAME="Canny";

const int CANNY_LOWER_BOUND=50;
const int CANNY_UPPER_BOUND=250;
const int HOUGH_THRESHOLD=150;

const double Kp = 0.5;
const double Ki = 0.2;
const double Kd = 0.1;

int main()
{
    VideoCapture capture(CAM_PATH);
    //If this fails, try to open as a video camera, through the use of an integer param
    if (!capture.isOpened())
    {
        capture.open(atoi(CAM_PATH.c_str()));
    }

    double dWidth=capture.get(CV_CAP_PROP_FRAME_WIDTH);			//the width of frames of the video
    double dHeight=capture.get(CV_CAP_PROP_FRAME_HEIGHT);		//the height of frames of the video
    clog<<"Frame Size: "<<dWidth<<"x"<<dHeight<<endl;

    Mat image;
//    init();
//    controlLeft(FORWARD,50);
//    controlRight(FORWARD,50);
//    turnTo(0);
    double integral,error,derivative,previous_error = 0,results;
    int current_angle = 0;
    while(true)
    {
        capture>>image;
        if(image.empty())
            break;
        Mat contours;
        Canny(image,contours,CANNY_LOWER_BOUND,CANNY_UPPER_BOUND);

        // 显示初步轮廓处理之后的结果
        imshow(CANNY_WINDOW_NAME,contours);
        imwrite("1.jpg",contours);
        waitKey(1);

        vector<Vec2f> lines;
        HoughLines(contours,lines,1,PI/180,HOUGH_THRESHOLD);
        Mat result(image.size(),CV_8U,Scalar(255));
        image.copyTo(result);
        clog<<lines.size()<<endl;
        cout<<result.size<<endl;
//
//        float maxRad=-2*PI;
//        float minRad=2*PI;
        double middle = PI/2;
        //Draw the lines and judge the slope
        for(vector<Vec2f>::const_iterator it=lines.begin();it!=lines.end();++it)
        {

            float rho=(*it)[0];			//First element is distance rho
            float theta=(*it)[1];		//Second element is angle theta

            cout<<rho<<" "<<theta<<endl;
            // 如果是向右走，夹角应该是正数， 如果是向左走，夹角应该是负数
            if(theta>middle) {
                theta = theta - PI;
            }
            error = theta;
            integral = integral + error;
            if (error == 0) {
                integral = 0;
            }
            if ( abs(error) > 40) {
                integral = 0;
            }
            derivative = error - previous_error;
            previous_error = error;
            results = Kp*error + Ki*integral + Kd*derivative;
            // 这里的result 就是得出的应该调整的角度 调整-result
            int angle = -(results*90/PI);
            current_angle+=angle;
//            turnTo(current_angle);

            // 这里直接把两条线输入进去然后产生结果

            if(true||(theta>0.09&&theta<1.48)||(theta>1.62&&theta<3.05))
            {
//                if(theta>maxRad)
//                    maxRad=theta;
//                if(theta<minRad)
//                    minRad=theta;

                //point of intersection of the line with first row
                Point pt1(rho/cos(theta),0);
                //point of intersection of the line with last row
                Point pt2((rho-result.rows*sin(theta))/cos(theta),result.rows);
                //Draw a line
                line(result,pt1,pt2,Scalar(0,255,255),3,CV_AA);
            }
            clog<<"Line: ("<<rho<<","<<theta<<")\n";
        }
        stringstream overlayedText;
        overlayedText<<"Lines: "<<lines.size();
        putText(result,overlayedText.str(),Point(10,result.rows-10),2,0.8,Scalar(0,0,255),0);
        imshow(MAIN_WINDOW_NAME,result);
        imwrite("0.jpg", result);
        lines.clear();
//        stopLeft();
//        stopRight();
        break;
        waitKey(1);
    }
    return 0;
}
