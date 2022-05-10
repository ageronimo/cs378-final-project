#include <ros/ros.h>
#include <race/drive_param.h>
#include <race/pid_input.h>
#include <ros/callback_queue.h>
#include <race/test.h>

#include <ctime>
#include <chrono>
#include <thread>

#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <std_msgs/String.h>
#include <sstream>
using namespace std;

int main(int argc, char **argv){
	
	ros::init(argc, argv, "hw1pub");
	
	int pub_queue_size = 1;
	ros::NodeHandle p;
	ros::Publisher pub = p.advertise<std_msgs::String>("CS378TEST", pub_queue_size);

	int counter = 0;
	while(1) {
		//if ((counter % 200000)) {
			
			//race::test msg;
			std_msgs::String msg;
			std::stringstream ss;
			int ranNum = rand() % 10;
			//msg.test = "TestParameter " << ranNum;
			ss << "TestParameter " << ranNum;
			msg.data = ss.str();
			msg.data = "TurnLeft";
			cout << msg.data.c_str() << endl;
			pub.publish(msg);
			counter = 0;
			sleep(15);
		//}
		counter++;
	}
	return 0;
}
