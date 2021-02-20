#include<iostream>
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

int main(int argc, char const *argv[])
{
	Mat traffic;
	traffic = imread("traffic.jpg", IMREAD_GRAYSCALE);
    // Four corners of the book in source image
    vector<Point2f> pts_src;
    pts_src.push_back(Point2f(900,250));
    pts_src.push_back(Point2f(150,980));
    pts_src.push_back(Point2f(1570,980));
    pts_src.push_back(Point2f(1300,250));

    vector<Point2f> pts_dest;
    pts_dest.push_back(Point2f(472,52));
    pts_dest.push_back(Point2f(472,830));
    pts_dest.push_back(Point2f(800,830));
    pts_dest.push_back(Point2f(800,52));

    Mat h = findHomography(pts_src, pts_dest);
    Mat t_out, t_out_cropped;
    warpPerspective(traffic, t_out, h, traffic.size());

    Rect crop(447, 10, 400, 700);
    t_out_cropped = t_out(crop);

    namedWindow("Warped", WINDOW_NORMAL);
    imshow("Warped", t_out);
    waitKey(0);
    destroyAllWindows();
    imshow("Warped", t_out_cropped);
    waitKey(0);
    imwrite("traffic_out.jpg", t_out_cropped);
	return 0;
}