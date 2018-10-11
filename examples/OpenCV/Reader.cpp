//
// Created by 刘瑷玮 on 2018/9/30.
//
#include <cstdlib>
#include <iostream>
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
const string CAM_PATH="/dev/video0";
const string MAIN_WINDOW_NAME="Processed Image";
const string CANNY_WINDOW_NAME="Canny";

const int CANNY_LOWER_BOUND=50;
const int CANNY_UPPER_BOUND=250;
const int HOUGH_THRESHOLD=150;
const double PI = 3.14159;

class LineFinder
{
private:
    cv::Mat img;
    //包含被检测直线的端点的向量
    std::vector<cv::Vec4i> lines;

    //累加器分辨率参数
    double deltaRho;
    double deltaTheta;

    //确认直线之前必须受到的最小投票数
    int minVote;

    //直线的最小长度
    double minLength;
    //直线上允许的最大空隙
    double maxGap;
public:
    LineFinder() :deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.0), maxGap(0.0) {}
    void setAccResolution(double dRho, double dTheta)
    {
        deltaRho = dRho;
        deltaTheta = dTheta;
    }
    void setminVote(int minv)
    {
        minVote = minv;
    }
    void setLengthAndGap(double length, double gap)
    {
        minLength = length;
        maxGap = gap;
    }
    std::vector<cv::Vec4i> findLines(cv::Mat& binary)
    {
        lines.clear();
        cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
        return lines;
    }
    void drawDetectedLines(cv::Mat& image, cv::Scalar color = cv::Scalar(255, 255, 255))
    {
        std::vector<cv::Vec4i>::const_iterator it = lines.begin();
        while (it!=lines.end())
        {
            cv::Point pt1((*it)[0], (*it)[1]);
            cv::Point pt2((*it)[2], (*it)[3]);
            cv::line(image, pt1, pt2, color);
            it++;
        }
    }
};


int main(){
    LineFinder cFinder;
    cFinder.setLengthAndGap(80, 20);
    cFinder.setminVote(60);
    for(int i = 0;i <= 310; i++) {
//        Mat image = imread("../../image/"+to_string(i)+".jpg");
        Mat image = imread("../../test.png");
        Mat contours;
        Canny(image, contours, CANNY_LOWER_BOUND, CANNY_UPPER_BOUND);
        Mat contoursInv;
        threshold(contours, contoursInv, 128, 255, THRESH_BINARY_INV);
        imwrite("1.jpg", contoursInv);

        std::vector<Vec2f> lines;
        int houghVote = 100;
        if (houghVote < 1 or lines.size() > 2) { // we lost all lines. reset
            houghVote = 200;
        } else { houghVote += 25; }
        while (lines.size() < 5 && houghVote > 0) {
            HoughLines(contours, lines, 1, PI / 180, houghVote);
            houghVote -= 5;
        }
        std::cout << houghVote << "\n";
        Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));
        image.copyTo(result);

        std::vector<Vec2f>::const_iterator it = lines.begin();
        Mat hough(image.size(), CV_8U, Scalar(0));
        cout << PI / 10. << " " << 19. * PI / 10. << endl;
        double min = 10000, max = 0;
        while (it != lines.end()) {

            float rho = (*it)[0];   // first element is distance rho
            float theta = (*it)[1]; // second element is angle theta
            if (theta < 1.3 || theta >
                               1.9) { // filter theta angle to find lines with theta between 30 and 150 degrees (mostly vertical)
                min = theta < min ? theta : min;
                max = theta > max ? theta : max;
                // point of intersection of the line with first row
                Point pt1(rho / cos(theta), 0);
                // point of intersection of the line with last row
                Point pt2((rho - result.rows * sin(theta)) / cos(theta), result.rows);
                // draw a white line
                line(result, pt1, pt2, Scalar(255), 3);
                line(hough, pt1, pt2, Scalar(255), 3);
            }
            std::cout << "line: (" << rho << "," << theta << ")\n";
            ++it;
        }
        cout << max << " " << min << endl;
        LineFinder ld;

        // Set probabilistic Hough parameters
        ld.setLengthAndGap(60, 10);
        ld.setminVote(4);

        // Detect lines
        std::vector<Vec4i> li = ld.findLines(contours);
        Mat houghP(image.size(), CV_8U, Scalar(0));
        ld.drawDetectedLines(houghP);
//        imwrite("3.jpg", houghP);

//        imwrite("2.jpg", result);
        Mat r;
        bitwise_and(hough, houghP, r);
//        imwrite("4.jpg", r);


//    imwrite("0.jpg", contours);
        vector<cv::Vec2f> lines1;
        HoughLines(r, lines1, 1, PI / 180, 90);//最小投票数
//    vector<Vec2f>::const_iterator it = lines.begin();
        Mat result1(image.size(), CV_8U, Scalar(255));
        image.copyTo(result1);
//    cFinder.drawDetectedLines(result);
        for (vector<Vec2f>::const_iterator it = lines1.begin(); it != lines1.end(); ++it) {

            float rho = (*it)[0];            //First element is distance rho
            float theta = (*it)[1];        //Second element is angle theta
            cout << rho << " " << theta << endl;
            //point of intersection of the line with first row
            Point pt1(rho / cos(theta), 0);
            //point of intersection of the line with last row
            Point pt2((rho - result1.rows * sin(theta)) / cos(theta), result1.rows);
            //Draw a line
            line(result1, pt1, pt2, Scalar(0, 255, 255), 3, CV_AA);
            clog << "Line: (" << rho << "," << theta << ")\n";
        }
        imwrite("image/"+to_string(i)+".jpg", result1);
        break;
    }

    return 0;

}
