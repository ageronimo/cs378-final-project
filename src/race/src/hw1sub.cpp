#include <ros/ros.h>
#include <race/drive_param.h>
#include <race/pid_input.h>
#include <race/test.h>
#include <ros/callback_queue.h>

#include <ctime>
#include <chrono>
#include <thread>

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <std_msgs/String.h>
using namespace std;


void callBack(const std_msgs::String::ConstPtr& msg){
	cout << msg->data.c_str() << endl;
}



int main(int argc, char **argv){
	ros::init(argc, argv, "hw1sub");
	int sub_queue_size = 1;
	ros::NodeHandle s;
	ros::Subscriber sub = s.subscribe<std_msgs::String>("CS378TEST", sub_queue_size, callBack);
	ros::spin();
	return 0;
}
