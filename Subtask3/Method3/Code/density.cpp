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
using namespace cv;
using namespace std;
using namespace chrono;

struct backsub{                             //structure to be passed onto while creating thread.
    Mat image;                              //stores background image, background subtractor pointer
    Ptr<BackgroundSubtractor> sub;          //pixels and num_pixel store return values of non-black pixel
    double pixel;                           // and total pixels
    double num_pixel;
};

void *density(void *x){
    struct backsub *my_struct = (struct backsub*) x;        //performs background subtraction for given part of frame
    Mat fgMask1;                                                
    my_struct->sub->apply(my_struct->image,fgMask1,0);  
    my_struct->pixel = (countNonZero(fgMask1));
    my_struct->num_pixel = double(fgMask1.total());         //updates the num_pixel and pixel
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    string name = argv[1];                                  //storing the name of video
    int n = stoi(argv[2]);                                  //n->Indicating range of values NUM_THREADS will take.
    ofstream file;
    file.open("../Analysis/Num_of_threads_vs_runtime.txt");     //create a file to store runtime vs NUM_THREADS
    for (int NUM_THREADS = 1; NUM_THREADS <= n; NUM_THREADS++)  //perform for NUM_THREADS from 1 to n.
    {
        VideoCapture capture(name + ".mp4");                //open the video
        if (!capture.isOpened()){                           //checking if video exists, returning error otherwise
            cout << "This video: "<< name <<" is not in the same directory! Put the video in the same directory or give the correct video name." << endl;
            return 0;
        }

        Mat frame = imread("Background.jpg");               //reading empty image of road for using as background
        if (frame.empty()) {                                //ensures that image exists in the folder
            cout<<"Please enter the correct background image name.\n";
            return -1;
        }
        double pixels;
        ofstream myfile;
        myfile.open("../Analysis/Outfiles/output_"+to_string(NUM_THREADS)+".txt");
        myfile <<  "Time" << "," << "Queue Density" << "\n"; 
        Mat fgMask;
        Mat frames[NUM_THREADS];                            //Initialise NUM_THREADS number of threads 
        pthread_t threads[NUM_THREADS];                     //Initialise NUM_THREADS number of structure to be passed
        struct backsub pass_struct[NUM_THREADS];            
        pthread_attr_t attr;                                //Initialise attribute of NUM_THREADS and set detachstate JOINABLE.
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        void *status;                                   
        int height,width,rc ;                              
        double qd;

        height = frame.rows;                                //Set height and width of frame to variables
        width = frame.cols;

        for (int i = 0; i < NUM_THREADS; i++)               //Cut the initial frame into NUM_THREAD parts
        {
            Rect crop(0, height*i/NUM_THREADS, width, height/NUM_THREADS);
            frames[i] = frame(crop);
        }
        
        Ptr<BackgroundSubtractor> pBackSubs[NUM_THREADS];
        for (int i = 0; i < NUM_THREADS; i++)                // Initialise the array of Background subtractor
        {
            pBackSubs[i] = createBackgroundSubtractorMOG2(500,128,false);  
            pBackSubs[i]->apply(frames[i], fgMask, 0);       // 
            pass_struct[i].sub = pBackSubs[i]; 
        }
        myfile << 0 << "," << 1 << "\n";     
        auto startTime = high_resolution_clock::now();
        while (true) {
            int total = 0;
            capture >> frame;                               //reading next frames

            if (frame.empty())                              //stop if video has ended
                break;

            frame = angle_correction(frame);                //correcting perception using code of previous subtask

            stringstream ss;                                //reading frame_number and storing it as string
            ss << capture.get(CAP_PROP_POS_FRAMES);
            string frameNum = ss.str();
            float Time = (float)stoi(frameNum)/15;          //obtain the frame-number

            for (int i = 0; i < NUM_THREADS; i++)           //split the current frame to NUM_THREAD parts.
            {
                Rect crop(0, height*i/NUM_THREADS, width, height/NUM_THREADS);
                frames[i] = frame(crop);
            }

            for( int i = 0; i < NUM_THREADS; i++ ) {           //create NUM_THREAD threads and assign each of them
                pass_struct[i].image = frames[i];              //work to process the given frame parallely
                rc = pthread_create(&threads[i], &attr, density, &pass_struct[i]);
                if (rc) {
                    cout << "Error:unable to create thread," << rc << endl;
                    exit(-1);
                }
            }

            for( int i = 0; i < NUM_THREADS; i++ ) {           //Wait for each thread to complete its work
                rc = pthread_join(threads[i], &status);
                if (rc) {
                    cout << "Error:unable to join," << rc << endl;
                    exit(-1);
                }
            }

            for (int i = 0; i < NUM_THREADS; i++)              //add the required pixel-counts and obtain the queue density.
            {
                qd += pass_struct[i].pixel ;
                total += pass_struct[i].num_pixel;
            }
            qd = qd/total;
            myfile << Time << "," << qd << "\n";                //write the queue density.
            qd = 0.0;
        }
        pthread_attr_destroy(&attr);                            //destroy the attribute 
        auto stopTime = high_resolution_clock::now();           //record the finsh time for current number of threads.
        auto duration = duration_cast<microseconds>(stopTime - startTime);    
        myfile.close();                                         
        file << NUM_THREADS << "," << duration.count()/1000000.0 << endl; //write the time for current NUM_THREADS. 
        cout<<"Execution completed for NUM_THREADS = "<<NUM_THREADS<<endl; 
    }
    file.close();
    return 0;
}