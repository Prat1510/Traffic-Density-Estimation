#include<iostream>
#include "opencv2/opencv.hpp"
#include<string>
using namespace cv;
using namespace std;

void on_mouse( int event, int x, int y, int flag, void *ptr )  //function that takes mouse click inputs 
{
    if (event == EVENT_LBUTTONDOWN )                           //only left click of mouse is input
    { 
        vector<Point2f>* pts_src = (vector<Point2f>*) ptr;
        pts_src->push_back(Point2f(x,y));                      //adding the input in the source points

        if (pts_src->size() == 4)                              //only 4 points are taken as input
        {
        destroyAllWindows();
        }
    }
}

int main(int argc, char const *argv[])
{	
    if (argc == 1)                                            //ensures that name of image is given
    {
        cout<<"Please provide the input image name as an arguement along with the program name.\n";
        return -1;
    }
	if (argc > 2)                                             //ensuring that extra names are not given
    {
        cout<<"Only one arguement has to be provided specifying the name of input image.\n";
        return -1;
    }

    string name = argv[1];
	Mat image;
	image = imread(name + ".jpg", IMREAD_GRAYSCALE);          //reading the desired and converting 
															  //into grayscale
    if (image.empty()) {                                      //ensures that image exists in the folder
        cout<<"Please enter the correct image file name.\n";
        return -1;
    }

    vector<Point2f> pts_src;

    namedWindow("Original Frame", WINDOW_AUTOSIZE);           //taking the source points from mouse click
    imshow("Original Frame", image);
    setMouseCallback("Original Frame", on_mouse, &pts_src);
    waitKey(0);

    vector<Point2f> pts_dest;                                 //putting the destination points
    pts_dest.push_back(Point2f(472,52));
    pts_dest.push_back(Point2f(472,830));
    pts_dest.push_back(Point2f(800,830));
    pts_dest.push_back(Point2f(800,52));

    Mat h = findHomography(pts_src, pts_dest);                //using inbuilt function for finding homography
    Mat M(875, 1280, CV_8UC1, Scalar(0));                     //creating a matrix of appropriate size
    Mat t_out, t_out_cropped;
    
    warpPerspective(image, t_out, h, M.size());               //changing the perspective to the top view using homography

    Rect crop(472, 52, 328, 778);
    t_out_cropped = t_out(crop);                              //making cropped image

    namedWindow("Projected Frame", WINDOW_AUTOSIZE);          //showing and saving Projected image
    imshow("Projected Frame", t_out);
    waitKey(0);
    imwrite(name + "_projected.jpg", t_out);

    destroyAllWindows();

    namedWindow("Cropped Frame", WINDOW_AUTOSIZE);            //showing and saving cropped image
    imshow("Cropped Frame", t_out_cropped); 
    waitKey(0);
    imwrite(name + "_cropped.jpg", t_out_cropped);

	return 0;
}