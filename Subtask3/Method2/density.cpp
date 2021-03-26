#include "cam.h"
#include "error.h"
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
    VideoCapture capture(name + ".mp4"); 
    for (int i = 1; i <= 2595; i++)
    {
        capture >> frame;  
    }

    frame = angle_correction(frame, X, Y);
    imwrite("background_" + to_string(X) + "_" + to_string(Y) + ".jpg" , frame);
}

int processVideo(string name, int X,int Y )
{

    Ptr<BackgroundSubtractor> pBackSub1;        //creating two backgroundSubstractor instances and
    Ptr<BackgroundSubtractor> pBackSub2;        //two pointers p[ointing to each of them
    pBackSub1 = createBackgroundSubtractorMOG2(500,128,false);     
    pBackSub2 = createBackgroundSubtractorMOG2(300,16,false);

    VideoCapture capture(name + ".mp4");        //open the video
    if (!capture.isOpened()){                   //cheacking if video exists, returning error otherwise
        cout << "This video: "<< name <<" is not in the same directory! Put the video in the same directory or give the correct video name." << endl;
        return 0;
    }

    Mat frame = imread("Background_" + to_string(X) + "_" + to_string(Y) + ".jpg");       //reading empty image of road for using as background
    if (frame.empty()) {                        //ensures that image exists in the folder
        cout<<"Please enter the correct background image name.\n";
        return -1;
    }
    double pixels;
    Mat fgMask1, fgMask2;
    ofstream myfile;
    myfile.open("output_" + to_string(X) + "_" + to_string(Y) + ".txt");
    myfile <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n"; 
    // cout <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n";
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
        // cout <<  Time << "," << whitePixels1 << "," << whitePixels2 << "\n";
        
        myfile << Time << "," << whitePixels1 << "," << whitePixels2 << "\n";      
        
        int keyboard = waitKey(20);             //stopping if esc is pressed on keyboard
        if (keyboard == 27)                 
            break;

        capture>>frame;
        capture>>frame;
        capture>>frame;
        
        if (frame.empty())                      //stop if video has ended
            break;

        frame = angle_correction(frame, X, Y);        //correcting perception using code of previous subtask
    }

    myfile.close();
    return 0;
}

int main(int argc, char const *argv[])
{
    string name = argv[1];
    int n;
    cout << "Enter n: ";
    cin >> n;
    vector<int> X_val; vector<int> Y_val;
    string str;

    for (int i = 0; i<n; i++)
    {
        cout << "Enter the " << i + 1 << " resolution\n";
        cin>>str;
        vector<string> temp;
        boost::split(temp, str, boost::is_any_of("x"));
        X_val.push_back(stoi(temp[0])); 
        Y_val.push_back(stoi(temp[1]));
    }

    ofstream file;
    file.open("Error.txt"); 
    for (int i = 0; i < n; i++)
    {
        makeBackground(name, X_val[i], Y_val[i]);
    }
    for (int i = 0; i<n; i++){
        auto startTime = high_resolution_clock::now(); 
        processVideo(name, X_val[i], Y_val[i]);
        auto stopTime = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stopTime - startTime);

        float arr[2]; 
        ErrorMeasure(to_string(X_val[i]) + "_" + to_string(Y_val[i]),arr);
        file<<X_val[i]<<"_"<<Y_val[i]<<","<<arr[0]<<","<<arr[1]<<","<<duration.count()/1000000<<"\n";
    }
}