#include "cam.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <fstream>
#include <chrono>
#include <boost/algorithm/string.hpp>
using namespace boost::algorithm;
using namespace cv;
using namespace std;
using namespace chrono;

void makeBackground(string name, int X, int Y){
    Mat frame;
    VideoCapture capture(name + ".mp4");            //captures the framenumber 2595 which is a empty frame
    for (int i = 1; i <= 2595; i++)                 
    {
        capture >> frame;  
    }                                                   
    frame = angle_correction(frame, X, Y);          //performs angle correction on the frame according to given 
                                                    //resolution and saves it into the analysis folder.
    imwrite("../Analysis/Outfiles/background_" + to_string(X) + "_" + to_string(Y) + ".jpg" , frame);
}

int processVideo(string name, int X,int Y )
{

    Ptr<BackgroundSubtractor> pBackSub1;        //creating a backgroundSubstractor instance and initialise it.
    
    pBackSub1 = createBackgroundSubtractorMOG2(500,128,false);     

    VideoCapture capture(name + ".mp4");        //open the video
    if (!capture.isOpened()){                   //cheacking if video exists, returning error otherwise
        cout << "This video: "<< name <<" is not in the same directory! Put the video in the same directory or give the correct video name." << endl;
        return 0;
    }

    Mat frame = imread("../Analysis/Outfiles/background_" + to_string(X) + "_" + to_string(Y) + ".jpg");       //reading empty image of road for using as background
    if (frame.empty()) {                                                                              //ensures that image exists in the folder
        cout<<"Please enter the correct background image name.\n";
        return -1;
    }
    double pixels;
    Mat fgMask1;
    ofstream myfile;
    myfile.open("../Analysis/Outfiles/output_" + to_string(X) + "_" + to_string(Y) + ".txt");
    myfile <<  "Time" << "," << "Queue Density" << "\n";                //create an output file in analysis folder 

    double whitePixels1;             

    while (true) {
        pixels = double(frame.total());

        pBackSub1->apply(frame, fgMask1, 0);    

        stringstream ss;                                //reading framenumber and storing it as string
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNum = ss.str();

        // imshow("Frame", frame);
        // imshow("FG Mask1", fgMask1);
        // imshow("FG Mask2", fgMask2);

        whitePixels1 = (countNonZero(fgMask1))/pixels ;   //counting no of non-black pixels and then priting values on console
        
        float Time = (float)stoi(frameNum)/15;
        
        myfile << Time << "," << whitePixels1<< "\n";     //writing queue density into the file
        
        // int keyboard = waitKey(20);                    //stopping if esc is pressed on keyboard
        // if (keyboard == 27)                 
        //     break;

        capture>>frame;                                   //take the next frame
        
        if (frame.empty())                                //stop if video has ended
            break;

        frame = angle_correction(frame, X, Y);            //correcting perception using code of previous subtask
    }

    myfile.close();
    return 0;
}

int main(int argc, char const *argv[])
{
    string name = argv[1];                              //read the videoname
    int n = stoi(argv[2]);                              //n: number of resolutions for which code is run
    vector<int> X_val; vector<int> Y_val;
    string str;
    double diff = 300.0/(n-1) ;                         
    double factor = 778.0/328.0;                        //equally split and append the vectors with X and Y
    for (int i = 0; i < n; i++)                         //of resolutions for which whole code is run.
    {
        double x = 328.0 - diff*i;
        X_val.push_back((int)x); 
        Y_val.push_back((int)(factor*x));
    }

    ofstream file;
    file.open("../Analysis/runtimes.txt");              //create a runtime vs resolution file.
    for (int i = 0; i < n; i++)
    {
        makeBackground(name, X_val[i], Y_val[i]);       //make background images for eac resolution.
    }
    for (int i = 0; i<n; i++){
        auto startTime = high_resolution_clock::now();  //record starting for i'th resolution
        processVideo(name, X_val[i], Y_val[i]);         //process for i'th resolution
        auto stopTime = high_resolution_clock::now();   //record the end time for i'th resolution
        auto duration = duration_cast<microseconds>(stopTime - startTime);
        cout << "Execution completed for resolution = " << X_val[i]<<"x"<<Y_val[i]<<endl;   
        file<<X_val[i]<<"_"<<Y_val[i]<<","<<duration.count()/1000000.0<<"\n";  //write resolution and duration for i'th iteration
    }
    file.close();
    return 0;
}