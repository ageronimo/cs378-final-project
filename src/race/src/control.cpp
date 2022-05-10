#include <ros/ros.h>
#include <race/drive_param.h>
#include <sensor_msgs/LaserScan.h>
#include <race/pid_input.h>
#include <ros/callback_queue.h>

#include <ctime>
#include <chrono>
#include <thread>
#include <stack>

#include <string>
#include <vector>
#include <iostream>
#include <iterator>
#include <iomanip>
#include <sstream>

#include <race/test.h>
#include <ros/callback_queue.h>
#include <cstdlib>
#include <std_msgs/String.h>
using namespace std;

static double kp = 250.0;
static double kd = 0.0;
static double servo_offset = 18.5;
static double prev_error = 0.0;
static double vel_input = 0.0;
static double start_time = 0.0;
//string previousCommands[100];
//int recordingIndex = -1;
static int choice = 0;	// choose spin mode


static auto end_time = std::chrono::high_resolution_clock::now();

// default values
static double def_speed = 18.0; 
static double def_angle = 0.0; 
static int def_dir = 1;
// current values
static double cur_speed = 0.0;
static double cur_angle = 0.0;
static double cur_dist = 0.0;

ros::Publisher pub; 

// recording 
auto timer = std::chrono::steady_clock::now();
static bool record = false;
struct recordedMove {
	string command; // command
	int dir; 		// +/- 1
	int time;		// microseconds
};
stack<recordedMove> cmdStack;

void processCommand(string command, int dir);

void change_speed(double speed) {
	cur_speed = speed;
	
	race::drive_param params;
	params.angle = cur_angle;
	params.velocity = cur_speed;
	pub.publish(params);
}

void change_angle(double angle) {
	cur_angle = angle;
	
	race::drive_param params;
	params.angle = cur_angle;
	params.velocity = cur_speed;
	pub.publish(params);
}

void do_circle() {
	cur_speed = 11.0;
	cur_angle = 100.0;
	
	race::drive_param params;
	params.angle = cur_angle;
	params.velocity = cur_speed;
	pub.publish(params);

}

void drive(int dir) {
	change_angle(def_angle);
	change_speed(dir * def_speed);

	// reset timer
	timer = std::chrono::steady_clock::now();
}

void stop() {
	change_speed(0.0);
}

void turnLeft(int dir){
	// dir positive if moving forward, negative o.w.
	// apply dir to speed only	
	// speed range: 11+, angle range: +/- 50
	change_angle(-75.0);
	sleep(3);
	// reset speed and angle
	drive(dir);
}

void turnRight(int dir) {
	change_angle(75.0);
	sleep(3);
	// reset speed and angle
	drive(dir);
}

void undo() {
	// undo forward movement before undo	
	recordedMove m;
	// get elapsed time microseconds
	m.time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - timer).count();
	m.command = "Start";
	m.dir = -def_dir;
	cmdStack.push(m);

	// go through elements of stack and process commands
	// once stack is empty, reset record bool val to false
	// usleep in microseconds (10e-6)
	while (!cmdStack.empty()) {
		recordedMove mu = cmdStack.top();
		cmdStack.pop();
		cout << "Undo command: " << mu.command << endl;
		processCommand(mu.command, mu.dir);
		if (mu.command == "Start") {
			cout << "Reverse for " << (mu.time/1000) << " ms" << endl;
			usleep(mu.time);
		}
	}
	record = false;
}


void callBack(const std_msgs::String::ConstPtr& msg){
	string command = msg->data.c_str();
	if (!command.empty() && cur_dist <= 0.8) {
		cout << "Command: " << command << endl;
		cout << "Dist to shape: " << cur_dist << endl;

		// nts: ignore turns at certain distances
		if (command == "TurnRandom"){
			record = true;
		}
	
		processCommand(command, def_dir);
	
		if (record) {
			recordedMove m;
			m.command = command;
			m.dir = -def_dir;
			cmdStack.push(m);
		}	
	}	
}

void processCommand(string command, int dir){	
	// undo forward movement prior to turn
	if (record && (command == "TurnRandom" || command == "TurnLeft" || command == "TurnRight")) {
		recordedMove m;
		// get elapsed time microseconds
		m.time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - timer).count();
		m.command = "Start";
		m.dir = -dir;
		cmdStack.push(m);
	}

	// command processing
	if(command == "TurnRandom"){	
		recordedMove m;
		m.dir = dir;
		
		int choice = rand() % 2;
		if(choice == 1){
			turnLeft(dir);
			m.command = "TurnRight";
        } else {
        	turnRight(dir);
        	m.command = "TurnLeft";
        }
        
        cmdStack.push(m);
    }
	else if(command == "TurnLeft"){
		turnLeft(dir);
    } else if (command == "TurnRight") {
    	turnRight(dir);
    } else if (command == "Undo") {
    	undo();
    } else if (command == "Stop") {
    	stop();
    } else if (command == "Start") {
    	drive(dir);
    }
}

void scanCallback(const boost::shared_ptr<const sensor_msgs::LaserScan> &data) {
	const sensor_msgs::LaserScan *raw_data = data.get();
	cur_dist = raw_data->ranges[540];
}

int main(int argc, char **argv){
	int pub_queue_size = 1;
	int sub_queue_size = 1;
	//int choice = 0;

	cout << "check 1\n";
		// Initiate Node
		ros::init(argc, argv, "pid_controller");
		// Create Node Handeler for publisher
		ros::NodeHandle p;
		pub = p.advertise<race::drive_param>("drive_parameters", pub_queue_size);
		race::drive_param params;
		
		// Create Node Handeler for subscriber
		
		//ros::NodeHandle s;
		//ros::Subscriber sub = s.subscribe<race::pid_input>("error", sub_queue_size, boost::bind(callback, _1, pub));
		//cout<< "intialized nodes\n";

		//listen for commands from camera
		//while(true){
		
	ros::init(argc, argv, "hw1sub");

    //ros::Subscriber sub = s.subscribe<std_msgs::String>("CS378TEST", sub_queue_size, callBack);
    ros::NodeHandle s;
	ros::Subscriber sub = s.subscribe<std_msgs::String>("ctcommands", sub_queue_size, callBack);
	
	ros::NodeHandle nh_scan;
	ros::Subscriber scanSub = nh_scan.subscribe<sensor_msgs::LaserScan>("scan", sub_queue_size, scanCallback);
        //}
		
	ros::spin();

	return 0;
}
