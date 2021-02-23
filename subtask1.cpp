#include<iostream>
#include "opencv2/opencv.hpp"
#include<string>
using namespace cv;
using namespace std;

void on_mouse( int event, int x, int y, int flag, void *ptr )
{
    if (event == EVENT_LBUTTONDOWN )
    { 
        vector<Point2f>*pts_src = (vector<Point2f>*)ptr;
        pts_src->push_back(Point2f(x,y));
    }
}

int main(int argc, char const *argv[])
{	
	string name = argv[1];
	Mat image;
	image = imread(name + ".jpg", IMREAD_GRAYSCALE);
    vector<Point2f> pts_src;

    namedWindow("Original Frame", WINDOW_NORMAL);
    setMouseCallback("Original Frame", on_mouse, &pts_src);
    imshow("Original Frame", image);
    waitKey(0);
    destroyAllWindows();

    vector<Point2f> pts_dest;
    pts_dest.push_back(Point2f(472,52));
    pts_dest.push_back(Point2f(472,830));
    pts_dest.push_back(Point2f(800,830));
    pts_dest.push_back(Point2f(800,52));

    Mat h = findHomography(pts_src, pts_dest);
    Mat t_out, t_out_cropped;
    warpPerspective(image, t_out, h, image.size());

    Rect crop(472, 52, 328, 778);
    t_out_cropped = t_out(crop);

    namedWindow("Projected Frame", WINDOW_NORMAL);
    imshow("Projected Frame", t_out);
    waitKey(0);
    imwrite(name + "_projected.jpg", t_out);
    destroyAllWindows();
    namedWindow("Cropped Frame", WINDOW_NORMAL);
    imshow("Cropped Frame", t_out_cropped);
    waitKey(0);
    imwrite(name + "_cropped.jpg", t_out_cropped);
	return 0;
}