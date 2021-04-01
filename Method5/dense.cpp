#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/photo.hpp>
#include "cam.h"
#include <fstream>
using namespace cv;
using namespace std;
int main()
{
    auto startTime = high_resolution_clock::now();                //record the starting time for i'th value of NUM_THREADS.
    VideoCapture capture("trafficvideo.mp4");
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    Mat frame1, prvs;
    double pixels;
    double whitePixels1;
    ofstream myfile;
    myfile.open("dense.txt");
    capture >> frame1;
    frame1 = angle_correction(frame1);
    cvtColor(frame1, prvs, COLOR_BGR2GRAY);
    pixels = double(prvs.total());

    while(true){
        Mat frame2, next;
        capture >> frame2;
        if (frame2.empty())
            break;
        frame2 = angle_correction(frame2);
        cvtColor(frame2, next, COLOR_BGR2GRAY);
        Mat flow(prvs.size(), CV_32FC2);
        calcOpticalFlowFarneback(prvs, next, flow, 0.5, 3, 25, 3, 5, 1.2, 0);
        // visualization
        Mat flow_parts[2];
        split(flow, flow_parts);
        Mat magnitude, angle, magn_norm;
        cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
        normalize(magnitude, magn_norm, 0.0f, 1.0f, NORM_MINMAX);
        angle *= ((1.f / 360.f) * (180.f / 255.f));
        //build hsv image
        Mat _hsv[3], hsv, hsv8, bgr;
        _hsv[0] = angle;
        _hsv[1] = Mat::ones(angle.size(), CV_32F);
        _hsv[2] = magn_norm;
        merge(_hsv, 3, hsv);
        hsv.convertTo(hsv8, CV_8U, 255.0);
        cvtColor(hsv8, bgr, COLOR_HSV2BGR);
        cvtColor(bgr, bgr, COLOR_BGR2GRAY);
        stringstream ss;  
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNum = ss.str();
        fastNlMeansDenoising   (bgr,bgr, 3, 7,21 );   
        threshold(bgr,bgr,8,255,THRESH_TOZERO);
        whitePixels1 = (countNonZero(bgr))/pixels;

        myfile<<(float)stoi(frameNum)/15<<","<<whitePixels1<<endl;

        imshow("frame2", bgr);
        prvs = next;
    }
    auto stopTime = high_resolution_clock::now();           //Note the finish time for i'th value of NUM_THREADS.
    auto duration = duration_cast<microseconds>(stopTime - startTime);  
    myfile.close();
    cout<<duration.count()<<endl;
    return 0;

}