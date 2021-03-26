#include "cam.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <fstream>
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{

    if (argc == 1)                              //ensures that name of image is given
    {
        cout<<"Please provide the video name as an arguement along with the program name.\n";
        return -1;
    }
    if (argc > 2)                               //ensuring that extra names are not given
    {
        cout<<"Only one arguement has to be provided specifying the name of input video.\n";
        return -1;
    }
    string name = argv[1];                      //storing the name of video
    
    Ptr<BackgroundSubtractor> pBackSub1;        //creating two backgroundSubstractor instances and
    Ptr<BackgroundSubtractor> pBackSub2;        //two pointers p[ointing to each of them
    pBackSub1 = createBackgroundSubtractorMOG2(500,128,false);     
    pBackSub2 = createBackgroundSubtractorMOG2(300,16,false);

    VideoCapture capture(name + ".mp4");        //open the video
    if (!capture.isOpened()){                   //cheacking if video exists, returning error otherwise
        cout << "This video: "<< name <<" is not in the same directory! Put the video in the same directory or give the correct video name." << endl;
        return 0;
    }

    Mat frame = imread("Background.jpg");       //reading empty image of road for using as background
    if (frame.empty()) {                        //ensures that image exists in the folder
        cout<<"Please enter the correct background image name.\n";
        return -1;
    }
    double pixels;
    Mat fgMask1, fgMask2;
    ofstream myfile;
    myfile.open("output.txt");
    myfile <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n"; 
    cout <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n";
    double whitePixels1, whitePixels2;             

    while (true) {
        pixels = double(frame.total());

        pBackSub1->apply(frame, fgMask1, 0);    
        pBackSub2->apply(frame, fgMask2);

        stringstream ss;                        //reading frame and storing it as string
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNum = ss.str();

        // imshow("Frame", frame);
        // imshow("FG Mask1", fgMask1);
        // imshow("FG Mask2", fgMask2);

        whitePixels1 = (countNonZero(fgMask1))/pixels ;   //counting no of white pixels and then priting values on console
        whitePixels2 = (countNonZero(fgMask2))/pixels ;
        float Time = (float)stoi(frameNum)/15;
        cout <<  Time << "," << whitePixels1 << "," << whitePixels2 << "\n";
        myfile << Time << "," << whitePixels1 << "," << whitePixels2 << "\n";        

        int keyboard = waitKey(20);             //stopping if esc is pressed on keyboard
        if (keyboard == 27)                 
            break;

        capture >> frame;                       //reading next frames, we are processing every third frame only 
        capture >> frame;                       //for faster execution of program
        capture >> frame;
        
        if (frame.empty())                      //stop if video has ended
            break;

        frame = angle_correction(frame);        //correcting perception using code of pevious subtask
    }

    myfile.close();
    return 0;
}


