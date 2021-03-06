#include "cam.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;
// const char* params
//     = "{ help h         |           | Print usage }"
//       "{ input          | vtest.avi | Path to a video or a sequence of image }"
//       "{ algo           | MOG2      | Background subtraction method (KNN, MOG2) }";
int main(int argc, char* argv[])
{
    // CommandLineParser parser(argc, argv, params);
    // parser.about( "This program shows how to use background subtraction methods provided by "
    //               " OpenCV. You can process both videos and images.\n" );
    // if (parser.has("help"))
    // {
    //     //print help information
    //     parser.printMessage();
    // }
    //create Background Subtractor objects
    Ptr<BackgroundSubtractor> pBackSub1,pBackSub2;
    // if (parser.get<String>("algo") == "MOG2")
    //     pBackSub = createBackgroundSubtractorMOG2();
    // else
    //     pBackSub = createBackgroundSubtractorKNN();
    pBackSub1 = createBackgroundSubtractorMOG2();
    pBackSub2 = createBackgroundSubtractorKNN();
    // pDynamic = createBackgroundSubtractorMOG2();
    VideoCapture capture("trafficvideo.mp4");
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open: Traffic-video" << endl;
        return 0;
    }
    Mat frame = imread("empty.jpg");
    Mat fgMask1, fgMask2;
    while (true) {
        //update the background model
        frame = angle_correction(frame);
        pBackSub1->apply(frame, fgMask1, 0);
        pBackSub2->apply(frame, fgMask2, 0.99);
        // pDynamic->apply(frame, fgMask, 0.99999);
        //get the frame number and write it on the current frame
        // rectangle(frame, Point(0, 0), Point(328, 778),
        //           Scalar(255,255,255), -1);
        stringstream ss;
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNumberString = ss.str();
        putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
                FONT_HERSHEY_SIMPLEX, 0.5 , cv::Scalar(0,0,0));
        //show the current frame and the fg masks
        imshow("Frame", frame);
        imshow("FG Mask1", fgMask1);
        imshow("FG Mask2", fgMask2);
        //get the input from the keyboard
        int keyboard = waitKey(200);
        if (keyboard == 'q' || keyboard == 27)
            break;
        capture >> frame;
        if (frame.empty())
            break;
    }
    return 0;
}