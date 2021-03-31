#include "cam.h"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <fstream>
#include <chrono>
using namespace cv;
using namespace std;
using namespace chrono;

struct bgs{
    int n;
    int part_num;
    string video_name;
} ;

void *density(void *x){
    struct bgs *my_struct = (struct bgs*) x;
    int NUM_THREADS = my_struct->n;
    int i = my_struct->part_num;
    string name = my_struct->video_name;
    Ptr<BackgroundSubtractor> pBackSub1;        //creating two backgroundSubstractor instances and
    // Ptr<BackgroundSubtractor> pBackSub2;        //two pointers p[ointing to each of them
    pBackSub1 = createBackgroundSubtractorMOG2(500,128,false);     
    // pBackSub2 = createBackgroundSubtractorMOG2(300,16,false);

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
    Mat fgMask1;
    ofstream myfile;
    string filename = "output" + to_string(NUM_THREADS) + "_"+ to_string(i) + ".txt";
    myfile.open("../analysis/" + filename);
    // cout <<  "Time" << "," << "Queue Density" << "," << "Dynamic Density" << "\n";
    double whitePixels1; 
    int frames_total = capture.get(CAP_PROP_FRAME_COUNT);            
    int end = frames_total*(i+1)/NUM_THREADS;
    capture.set(CAP_PROP_POS_FRAMES, frames_total*i/NUM_THREADS);
    while (true) {
        
        pixels = double(frame.total());

        pBackSub1->apply(frame, fgMask1, 0);    
        // pBackSub2->apply(frame, fgMask2);

        stringstream ss;                        //reading frame and storing it as string
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNum = ss.str();

        // imshow("Frame", frame);
        // imshow("FG Mask1", fgMask1);
        // imshow("FG Mask2", fgMask2);

        whitePixels1 = (countNonZero(fgMask1))/pixels ;   //counting no of white pixels and then priting values on console
        // whitePixels2 = (countNonZero(fgMask2))/pixels ;
        float Time = (float)stoi(frameNum)/15;
        // cout <<  Time << "," << whitePixels1 << "," << whitePixels2 << "\n";
        // for (int i = 0; i < x; ++i)
        // {
        myfile << Time << "," << whitePixels1<< "\n";      
        // }
        if (stoi(frameNum) == end && (i+1 != NUM_THREADS)){
            break;
        } 
        //stopping if esc is pressed on keyboard
        // if (keyboard == 27)                 
        //     break;

        capture>>frame;

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
    
    int n = stoi(argv[2]);
    ofstream file;
    file.open("../analysis/" + string("Threads_vs_runtime.txt"));
    for (int NUM_THREADS = 1; NUM_THREADS <= n; NUM_THREADS++)
    {
        auto startTime = high_resolution_clock::now();
        int rc;
        int i;
        pthread_t threads[NUM_THREADS];
        pthread_attr_t attr;
        void *status;
        struct bgs pass_struct[NUM_THREADS];
        // Initialize and set thread joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        for( i = 0; i < NUM_THREADS; i++ ) {
            pass_struct[i].n = NUM_THREADS;
            pass_struct[i].part_num = i;
            pass_struct[i].video_name = argv[1];
            rc = pthread_create(&threads[i], &attr, density, &pass_struct[i] );
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
          // cout << "Main: completed thread id :" << i ;
          // cout << "  exiting with status :" << status << endl;
        }
        auto stopTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stopTime - startTime);  
        cout << "Completed for NUM_THREADS :"<< NUM_THREADS << endl;
        file << NUM_THREADS<<","<< duration.count()/1000000.0<<"\n";
        ofstream output;
        output.open("../Analysis/output" + to_string(NUM_THREADS) + ".txt");
        for (int i = 0; i < NUM_THREADS; ++i)
        {
            ifstream curr;
            curr.open("../Analysis/output" + to_string(NUM_THREADS) + "_"+ to_string(i) + ".txt");
            string line;
            
            if (i != 0){
                getline(curr,line);
            }

            while (getline(curr,line)){
                output<<line<<"\n";
            }
            curr.close();
            string filename = string("../Analysis/output") + to_string(NUM_THREADS) + "_"+ to_string(i) + ".txt";
            remove(filename.c_str());
        }
        output.close();
    }
    file.close();
    return 0;
}


