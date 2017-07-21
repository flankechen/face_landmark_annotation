#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

//global number of landmarks
int g_landmark_n = 41;
Mat image,draw_image;
int click_count=0;
vector<Point> landmark_vec;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{

    if ( flags == (EVENT_FLAG_LBUTTON) )
    {
        cout << "Left mouse button is clicked while pressing CTRL key - position (" << x << ", " << y << ")" << endl;
        landmark_vec.push_back(Point(x,y));
        circle(draw_image, Point(x,y), 1, Scalar(0,255,0), -1);
        putText(draw_image, std::to_string(click_count), Point(x,y), FONT_HERSHEY_PLAIN, 2,  Scalar(0,0,255,255));
        imshow("draw_image", draw_image);
        click_count++;
        
       
    }
}



int main(int argc, char** argv )
{
    if ( argc != 2 )
    {
        printf("usage: ./face_annotation <Image_Path>\n");
        return -1;
    }


    image = imread( argv[1], 1 );
    draw_image = image.clone();
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }

    //make .pts file
    std::string file_string(argv[1]);
    file_string.replace(file_string.find("jpg"), sizeof("pts")-1, "pts");
    cout << file_string <<endl;
    ofstream pts_file;
    pts_file.open(file_string.c_str());

    pts_file << "version: 1" <<endl;
    pts_file << "n_points: "<< g_landmark_n <<endl;
    pts_file << "{"<<endl;

    namedWindow("Display Image", WINDOW_NORMAL );
    setMouseCallback("Display Image", CallBackFunc, NULL);
    imshow("Display Image", image);

    waitKey(0);
    if(landmark_vec.size()!=41)
    {
        cout<<"Not exactly 41 points is clicked, wrong things will be save to .pts"<<endl;
        return 0;
    }
    for(int i=0; i<g_landmark_n; i++)
    {
        pts_file <<landmark_vec[i].x <<" " <<landmark_vec[i].y<<endl;
    }
    pts_file << "}"<<endl;
    pts_file.close();
    return 0;
}
