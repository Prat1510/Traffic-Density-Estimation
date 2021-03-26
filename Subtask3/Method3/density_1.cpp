#include "cam.h"
// #include "error.h"
#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <fstream>
#include <chrono>
#define NUM_THREADS 7
using namespace cv;
using namespace std;
using namespace chrono;

void *density(void *x){
    long t;
    string name = "trafficvideo";
    t = (long)x;
    t = t + 1;
    Ptr<BackgroundSubtractor> pBackSub1;        //creating two backgroundSubstractor instances and
    Ptr<BackgroundSubtractor> pBackSub2;        //two pointers p[ointing to each of them
    pBackSub1 = createBackgroundSubtractorMOG2(500,128,false);     
    pBackSub2 = createBackgroundSubtractorMOG2(300,16,false);

    VideoCapture capture(name + ".mp4");        //open the video
    if (!capture.isOpened()){                   //cheacking if video exists, returning error otherwise
        cout << "This video: "<< name <<" is not in the same directory! Put the video in the same directory or give the correct video name." << endl;
        // return ;
    }

    Mat frame = imread("Background.jpg");       //reading empty image of road for using as background
    if (frame.empty()) {                        //ensures that image exists in the folder
        cout<<"Please enter the correct background image name.\n";
        // return ;
    }
    double pixels;
    Mat fgMask1, fgMask2;
    ofstream myfile;
    myfile.open("output" + to_string(t) + ".txt");
    myfile <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n"; 
    // cout <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n";
    double whitePixels1, whitePixels2;             
    int start = 0;

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
        // for (int i = 0; i < x; ++i)
        // {
            myfile << Time << "," << whitePixels1 << "," << whitePixels2 << "\n";      
        // }

        int keyboard = waitKey(20);             //stopping if esc is pressed on keyboard
        if (keyboard == 27)                 
            break;

        if (start == 0){
            for (int i = 0; i < t; i++)
            {
                capture>>frame;
            }
            start = 1;
        }
        else{
            for (int i = 0; i < 7; ++i)
            {
                capture>>frame;
            }   
        }

        if (frame.empty())                      //stop if video has ended
            break;

        frame = angle_correction(frame);        //correcting perception using code of pevious subtask
    }
    myfile.close();
    pthread_exit(NULL);
    // return ;
}

int main(int argc, char* argv[])
{
    // if (argc == 1)                              //ensures that name of image is given
    // {
    //     cout<<"Please provide the video name as an arguement along with the program name.\n";
    //     return -1;
    // }
    // if (argc > 2)                               //ensuring that extra names are not given
    // {
    //     cout<<"Only one arguement has to be provided specifying the name of input video.\n";
    //     return -1;
    // }
    auto startTime = high_resolution_clock::now();

    int rc;
    int i;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for( i = 0; i < NUM_THREADS; i++ ) {
      cout << "main() : creating thread, " << i << endl;
      rc = pthread_create(&threads[i], &attr, density, (void *)i );
      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for( i = 0; i < NUM_THREADS; i++ ) {
      rc = pthread_join(threads[i], &status);
      if (rc) {
         cout << "Error:unable to join," << rc << endl;
         exit(-1);
      }
      cout << "Main: completed thread id :" << i ;
      cout << "  exiting with status :" << status << endl;
    }

    cout << "Main: program exiting." << endl;

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);    
    cout << duration.count()/1000000<<endl;

    return 0;
}


