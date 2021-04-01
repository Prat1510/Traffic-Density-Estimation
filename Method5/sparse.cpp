#include <iostream>
#include <vector>
#include <sstream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include "cam.h"
#include <fstream>
using namespace cv;
using namespace std;
int main(int argc, char **argv)
{
    auto startTime = high_resolution_clock::now();                //record the starting time for i'th value of NUM_THREADS.   
    string filename = string(argv[1]) + ".mp4";
    VideoCapture capture(filename);
    if (!capture.isOpened()){
        //error in opening the video input
        cerr << "Unable to open file!" << endl;
        return 0;
    }
    // Create some random colors
    vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++)
    {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }
    Mat old_frame, old_gray;
    vector<Point2f> p0, p1;
    ofstream myfile;
    myfile.open("sparse.txt");
    capture >> old_frame;
    old_frame = angle_correction(old_frame);
    cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
    goodFeaturesToTrack(old_gray, p0, 0, 0.3, 7, Mat(), 7, false, 0.04);
    // Create a mask image for drawing purposes
    Mat mask = Mat::zeros(old_frame.size(), old_frame.type());
    Mat greymask = Mat::zeros(old_frame.size(), old_frame.type());
    vector <double> vec(20,0.0);
    double prev = 0;
    int count = 0;
    double whitePixels1, pixels;
    while(true){
        count++;
        Mat frame, frame_gray;
        capture >> frame;
        if (frame.empty())
            break;
        stringstream ss;  
        ss << capture.get(CAP_PROP_POS_FRAMES);
        string frameNum = ss.str();
        frame = angle_correction(frame);
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        // calculate optical flow
        vector<uchar> status;
        vector<float> err;
        TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);
        if (p0.empty() || stoi(frameNum)%15 == 0){
            goodFeaturesToTrack(frame_gray, p0,0, 0.3, 7, Mat(), 7, false, 0.04);
        }

        if (count > 100){
            mask = Mat::zeros(old_frame.size(), old_frame.type()); 
            count = 0;
        }

        calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15,15), 2, criteria);
        vector<Point2f> good_new ;
        
        for(uint i = 0; i < p0.size(); i++)
        {
            // Select good points
            if(status[i] == 1) {
                good_new.push_back(p1[i]);
                // draw the tracks
                line(mask,p1[i], p0[i], colors[i], 3);
                circle(frame, p1[i], 5, colors[i], -1);
            }
        }
        Mat img;
        add(frame, mask, img);
        cvtColor(mask, greymask, COLOR_BGR2GRAY);
        imshow("greymask",greymask);
        imshow("Frame", img);
        pixels = double(greymask.total());
        whitePixels1 = (countNonZero(greymask))/pixels;
        vec.erase(vec.begin());
        vec.push_back(whitePixels1);
        double avg = 0.0;
        for (auto& it : vec) {
            avg+= it;
        }
        avg = avg/20.0;
        if (count == 100)
        {
            prev  = avg;
        }
        if (stoi(frameNum)<50)
        {
            myfile<<(float)stoi(frameNum)/15.0<<","<< avg <<endl;
        }
        else{
            if (count > 50){
                myfile<<(float)stoi(frameNum)/15.0<<","<< avg <<endl;
            }
            else{
                myfile<<(float)stoi(frameNum)/15.0<<","<< prev <<endl;
            }
        }
        int keyboard = waitKey(10);
        if (keyboard == 'q' || keyboard == 27)
            break;
        // Now update the previous frame and previous points
        old_gray = frame_gray.clone();
        p0 = good_new;
    }
    auto stopTime = high_resolution_clock::now();           //Note the finish time for i'th value of NUM_THREADS.
    auto duration = duration_cast<microseconds>(stopTime - startTime);
    cout<<duration.count()<<endl;  
}