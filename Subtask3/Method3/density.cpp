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

struct backsub{
    Mat image;
    Ptr<BackgroundSubtractor> sub;
    double queue_density;
    double num_pixel;
};

void *density(void *x){
    struct backsub *my_struct = (struct backsub*) x;
    Mat fgMask1;
    my_struct->sub->apply(my_struct->image,fgMask1,0);  
    double whitePixels;
    whitePixels = (countNonZero(fgMask1))/my_struct->num_pixel ;   //counting no of white pixels and then priting values on console
    my_struct->queue_density = whitePixels;
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    string name = argv[1];                      //storing the name of video

    int NUM_THREADS = stoi(argv[2]);

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
    ofstream myfile;
    myfile.open("output.txt");
    myfile <<  "Time" << "," << "Queue Density" << "\n"; 
    Mat fgMask;
    Mat frames[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    struct backsub pass_struct[NUM_THREADS];
    pthread_attr_t attr;        
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    void *status;
    int height,width,rc ;
    double qd;

    height = frame.rows;
    width = frame.cols;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        Rect crop(0, height*i/NUM_THREADS, width, height/NUM_THREADS);
        frames[i] = frame(crop);
        pass_struct[i].num_pixel = double(frames[i].total());
    }
    
    Ptr<BackgroundSubtractor> pBackSubs[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pBackSubs[i] = createBackgroundSubtractorMOG2(500,128,false);  
        pBackSubs[i]->apply(frames[i], fgMask, 0);  
        pass_struct[i].sub = pBackSubs[i]; 
    }

    auto startTime = high_resolution_clock::now();
    while (true) {
        capture >> frame;                       //reading next frames

        if (frame.empty())                      //stop if video has ended
            break;

        frame = angle_correction(frame);        //correcting perception using code of pevious subtask

        stringstream ss;                        //reading frame and storing it as string
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNum = ss.str();
        float Time = (float)stoi(frameNum)/15;
        // for x in range(0, n_rows)
        //     image=frame[x*roi_height:(x+1)*roi_height, 0*roi_width: 1*roi_width];

        for (int i = 0; i < NUM_THREADS; i++)
        {
            Rect crop(0, height*i/NUM_THREADS, width, height/NUM_THREADS);
            frames[i] = frame(crop);
        }

        for( int i = 0; i < NUM_THREADS; i++ ) {
            pass_struct[i].image = frames[i];
            rc = pthread_create(&threads[i], &attr, density, &pass_struct[i]);
            if (rc) {
                cout << "Error:unable to create thread," << rc << endl;
                exit(-1);
            }
        }

        // pthread_attr_destroy(&attr);
        for( int i = 0; i < NUM_THREADS; i++ ) {
            rc = pthread_join(threads[i], &status);
            if (rc) {
                cout << "Error:unable to join," << rc << endl;
                exit(-1);
            }
        }

        for (int i = 0; i < NUM_THREADS; i++)
        {
            qd += pass_struct[i].queue_density ;
        }
        qd = qd/(double)NUM_THREADS;
        myfile << Time << "," << qd << "\n";    
        qd = 0.0;
    }
    pthread_attr_destroy(&attr);
    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);    
    cout << duration.count()/1000000.0<<endl;

    myfile.close();
    return 0;
}


// for y in range(0,imgheight,M):
//     for x in range(0, imgwidth, N):
//         y1 = y + M
//         x1 = x + N
//         tiles = im[y:y+M,x:x+N]

//         cv2.rectangle(im, (x, y), (x1, y1), (0, 255, 0))
//         cv2.imwrite("save/" + str(x) + '_' + str(y)+".png",tiles)

// cv2.imwrite("asas.png",im)