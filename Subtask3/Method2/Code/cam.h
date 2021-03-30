#include<iostream>
#include "opencv2/opencv.hpp"
#include<string>
using namespace cv;
using namespace std;

Mat angle_correction(Mat image, int X, int Y)
{
    vector<Point2f> pts_src;                                  //taking the source points from mouse click
    pts_src.push_back(Point2f(960,233));
    pts_src.push_back(Point2f(400,1050));
    pts_src.push_back(Point2f(1584,1080));
    pts_src.push_back(Point2f(1288,227));

    vector<Point2f> pts_dest;                                 //putting the destination points
    pts_dest.push_back(Point2f(472,52));
    pts_dest.push_back(Point2f(472,52 + Y));
    pts_dest.push_back(Point2f(472 + X,52 + Y));
    pts_dest.push_back(Point2f(472 + X,52));

    Mat h = findHomography(pts_src, pts_dest);                //using inbuilt function for finding homography
    Mat M(875, 1280, CV_8UC1, Scalar(0));                     //creating a matrix of appropriate size
    Mat t_out, t_out_cropped;
    
    warpPerspective(image, t_out, h, M.size());               //changing the perspective to the top view using homography

    Rect crop(472, 52, X, Y);
    t_out_cropped = t_out(crop);                              //making cropped image and then returning it
    return t_out_cropped;
}
