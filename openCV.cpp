#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include<vector>
#include <iostream>
#include <stdlib.h>
#include "Object.h"

using namespace std;
using namespace cv;



//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 1000;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";
bool person_detected;


string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void drawObject(vector<Object> peoples,Mat &frame){

	for(int i = 0; i<peoples.size(); i++){
	cv::circle(frame,cv::Point(peoples.at(i).getXPos(),peoples.at(i).getYPos()),10,cv::Scalar(0,0,255));
	cv::putText(frame,intToString(peoples.at(i).getXPos())+ " , " + intToString(peoples.at(i).getYPos()),cv::Point(peoples.at(i).getXPos(),peoples.at(i).getYPos()+20),1,1,Scalar(0,255,0));
	}
}
void morphOps(Mat &thresh){

	erode(thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    dilate( thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

    dilate( thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    erode(thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );

}
void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed, bool &person_detected){
	vector<Object> peoples;
	int object_counter=0;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object

	int numObjects = hierarchy.size();

	if (numObjects > 0) {

		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;


				if(area>MIN_OBJECT_AREA && area < MAX_OBJECT_AREA){

					Object person;
					person.setXPos(moment.m10/area);
					person.setYPos(moment.m01/area);

					if ((person.getXPos() > 205 && person.getXPos() < 520) && (person.getYPos() > 0 && person.getYPos() < 400) ){
						peoples.push_back(person);

					drawObject(peoples,cameraFeed);
					object_counter++;

					}
				}
			}
		}
		printf("%d\n",object_counter);
	}

	person_detected = (object_counter > 0) ? true : false;
}

void avgBCG(int &H, int &S, int &V, Mat imgOriginal){
    int counter = 0;
    Mat image=imgOriginal.clone();
        for (int i= 300; i<310;i++){
            for (int k= 300; k<310;k++){
                Mat HSV_PIXEL;
                Mat RGB_PIXEL=image(Rect(i,k,1,1));
                cvtColor(RGB_PIXEL, HSV_PIXEL,CV_BGR2HSV);
                Vec3b hsv=HSV_PIXEL.at<Vec3b>(0,0);
                H=H+hsv.val[0];
                S=S+hsv.val[1];
                V=V+hsv.val[2];
                counter++;
            }
        }
       H=H/counter;
       S=S/counter;
       V=V/counter;
     printf("H:%d, S:%d, V:%d C:%d\n",H,S,V,counter);

}

int main(int argc, char* argv[])
{

	Mat cameraFeed;

	Mat threshold;
	Mat HSV;
	int H,S,V;



	VideoCapture capture;

	capture.open(1);

	if ( !capture.isOpened()){  // if not success, exit program
		cout << "Cannot open the web cam" << endl;
		return -1;
	}


	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);

	while(1){
		//store image to matrix
		 bool bSuccess = capture.read(cameraFeed);

		 if (!bSuccess){ //if not success, break loop
			 cout << "Cannot read a frame from video stream" << endl;
			 break;
		 }

		 if (person_detected == false){
		 avgBCG(H, S, V, cameraFeed);
		 }

		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);



		inRange(HSV,Scalar(H-40, S-60, V-60), Scalar(H+40, S+60, V+60),threshold);
		bitwise_not(threshold, threshold);
		morphOps(threshold);
		imshow(windowName2,threshold);
		trackFilteredObject(threshold,HSV,cameraFeed,person_detected);


		imshow(windowName,cameraFeed);




		if (waitKey(5) == 27)
		       {
		            cout << "esc key is pressed by user" << endl;
		            break;
		       }
	}






	return 0;
}

