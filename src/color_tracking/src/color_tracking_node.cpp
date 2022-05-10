#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <cv_bridge/cv_bridge.h>
#include <ctime>
#include "std_msgs/Int16.h"
//#include "opencv2/opencv.hpp"
#include "opencv2/features2d.hpp"
#include <iostream>

//CHANGE
#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#include <csignal>
#include <cstdio>
#include <math.h>
#include "std_msgs/String.h"


using namespace std;
using namespace cv;

//#define MEASURE_TIME 1
#define COLOR_TRACKING 1

#ifdef MEASURE_TIME
clock_t t_begin = 0;
#endif

static int counter = 0;
static int goal = 1000;
static double time_record[] = {};

int detected_count = 0;

void cv_process_img(const Mat& input_img, Mat& output_img)
{
    Mat gray_img;
    cvtColor(input_img, gray_img, CV_RGB2GRAY);
    
    double t1 = 20;
    double t2 = 50;
    int apertureSize = 3;
    
    Canny(gray_img, output_img, t1, t2, apertureSize);
}

void cv_publish_img(image_transport::Publisher &pub, Mat& pub_img)
{
    //sensor_msgs::ImagePtr pub_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", pub_img).toImageMsg();
    sensor_msgs::ImagePtr pub_msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", pub_img).toImageMsg();
    pub.publish(pub_msg);
}

void cv_color_tracking(const Mat& input_img, ros::Publisher &controlPub, ros::Publisher &macroPub)
{
    //convert input image from RGB to HSV
    Mat imgHSV;
    cvtColor(input_img, imgHSV, CV_BGR2HSV);

    // color mask definitions
	Mat1b mask1; //red (low)
	Mat1b mask2; //red (high)
	Mat1b mask3; //green
	Mat1b mask4; //blue
	string colors[3] = {"red", "green", "blue"};

	inRange(imgHSV, Scalar(0, 100, 100), Scalar(10, 255, 255), mask1); //red (low)
	inRange(imgHSV, Scalar(160, 100, 100), Scalar(179, 255, 255), mask2); //red (high)
	inRange(imgHSV, Scalar(36, 25, 25), Scalar(86, 255, 255), mask3); //green
	inRange(imgHSV, Scalar(90, 50, 70), Scalar(128, 255, 255), mask4); //blue

	// make masks iterable
	vector<Mat1b> masks;
	masks.push_back(mask1 | mask2); //combine red masks
	masks.push_back(mask3);
	masks.push_back(mask4);

	// color/shape detection
	for (int m = 0; m < masks.size(); m++) {
		string color = colors[m];

		// pre-process images
		erode(masks[m], masks[m], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(masks[m], masks[m], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(masks[m], masks[m], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(masks[m], masks[m], getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		//TODO: get rid of noise

		// find contours
		vector<vector<Point>> contours;
		vector<Point> approx;
		findContours(masks[m], contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

		// check contours for shapes
		// maybe format cs_tracking publisher msg params as:
			// string shape
			// string color
		for (int i = 0; i < contours.size(); i++) {
			approxPolyDP(contours[i], approx, 0.02*arcLength(contours[i], true), true);
			// dummy print
			// if (contourArea(approx) > 1000) {
			//     cout << "Area: " << contourArea(approx) << endl;
			//     cout << "Vertices: " << approx.size() << endl;
			// }

			// TODO: set up publisher/subscriber
			// TODO: check forward distance in control.cpp using lidar
			// TODO: outline shapes

			// publish msg
			std_msgs::String msg;
	
			// triangle check
			if (approx.size() == 3 && fabs(contourArea(approx)) > 10000 && isContourConvex(approx)) {
				cout << "Detected: triangle ";

				// publish appropriate macro by color
				// turn left
				if (color == "red") {
					msg.data = "TurnLeft";
				// turn right
				} else if (color == "green") {
					msg.data = "TurnRight";
				// random turn
				} else if (color == "blue") {
					msg.data = "TurnRandom";
				}

				cout << color << endl;
				if (contourArea(approx) > 1000) {
				    cout << "Area: " << contourArea(approx) << endl;
				}
			}

			// circle check
			if (approx.size() >= 8 && fabs(contourArea(approx)) > 10000 && isContourConvex(approx)) {
				cout << "Detected: circle ";
		
				// stop
				if (color == "red") {
					msg.data = "Stop";
				// start
				} else if (color == "green") {
					msg.data = "Start";
				// undo
				} else if (color == "blue") {
					msg.data = "Undo";
				}

				cout << color << endl;
				if (contourArea(approx) > 1000) {
				    cout << "Area: " << contourArea(approx) << endl;
				}
			}

			macroPub.publish(msg);
		}
	}

	// display vision windows
	//cv::imshow("color_tracking_input_img", input_img);
	//imshow("red_tracking", masks[0]);
	//imshow("green_tracking", masks[1]);
	//imshow("blue_tracking", masks[2]);
    
    waitKey(1);
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg, image_transport::Publisher &pub, ros::Publisher &controlPub, ros::Publisher &macroPub)
{
    
    auto start = std::chrono::high_resolution_clock::now();
    cv_bridge::CvImageConstPtr cv_ori_img_ptr;
    try{
        Mat cv_ori_img = cv_bridge::toCvShare(msg, "bgr8")->image;
        Mat cv_output_img;
        
        cv_process_img(cv_ori_img, cv_output_img);
        
#ifdef COLOR_TRACKING
        cv_color_tracking(cv_ori_img, controlPub, macroPub);
#endif
        
        cv_publish_img(pub, cv_output_img);
        
#ifdef MEASURE_TIME
        clock_t t_end = clock();
        double delta_time= double(t_end - t_begin) / CLOCKS_PER_SEC;
        //cout << "Delta_t = " << 1/delta_time << "\n";
        //t_begin = t_end;
#endif
        
        //imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
        //imshow("view", cv_output_img);
        waitKey(30);
        
#ifdef MEASURE_TIME
        t_begin = clock();
#endif
    }catch(cv_bridge::Exception& e){
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }

    // process time measurement
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    time_record[counter] = (double)elapsed.count();
    counter += 1;

    if(counter == goal){
        double sum = 0.0;
        for(int i = 0; i < goal; i ++){
            sum += time_record[i];
        }
        double mean = sum/goal;
        const char *path = "/home/nvidia/catkin_ws/time_test/color_tracking_recording.txt";
        ofstream file;
        file.open(path, ios::app);
        file << "The mean is: " << mean*1000 << "ms" << endl;
        time_t now = time(0);
        char* dt = ctime(&now);
        file << dt << '\n' << '\n';
        counter = 0;
    }
}



int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    
    ros::NodeHandle nh;
    
    //namedWindow("Vview");
    startWindowThread();
    image_transport::ImageTransport it(nh);
    
    ros::NodeHandle nh_pub;
    image_transport::ImageTransport itpub(nh_pub);
    image_transport::Publisher pub = itpub.advertise("sample/cannyimg", 1);
    
    ros::NodeHandle node;
    uint32_t queue_size = 500;
    ros::Publisher controlPub = node.advertise<std_msgs::Int16>("imagecontrol", queue_size);

	// publish colors/shapes to control.cpp
	queue_size = 1;
	ros::NodeHandle nh_macro;
	ros::Publisher macroPub = nh_macro.advertise<std_msgs::String>("ctcommands", queue_size);
	
    
    
#ifdef MEASURE_TIME
    t_begin = clock();
#endif
    
    image_transport::Subscriber sub = it.subscribe("rgb/image_rect_color", 1, boost::bind(imageCallback, _1, pub, controlPub, macroPub));
   // ros::Subscriber controlSub = it.subscribe("rbg/image_rect_color")
    ros::spin();
    

    
    //destroyWindow("view");
    
}

