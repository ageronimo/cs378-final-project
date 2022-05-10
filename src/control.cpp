#include <ros/ros.h>
#include <race/drive_param.h>
#include <race/pid_input.h>
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
using namespace std;

static double kp = 250.0;
static double kd = 0.0;
static double servo_offset = 18.5;
static double prev_error = 0.0;
static double vel_input = 0.0;
static double start_time = 0.0;
string previousCommands[100];
int recordingIndex = -1;
static int choice = 0;	// choose spin mode


static auto end_time = std::chrono::high_resolution_clock::now();

ros::Publisher pub; 
static double cur_speed = 0.0;
static double cur_angle = 0.0;


void callBack(const std_msgs::String::ConstPtr& msg){
	string command = msg->data.c_str();
    cout << command << endl;

	if(recordingIndex == -1){
		recordingIndex++;
    }


	if(command == "TurnRandom"){
		//werent recording before 
		if(recording == -1){
			recordingIndex = 0;
        }
		
		int choice = rand() % 2;
		if(choice == 1){
			turnLeft();
			previousCommands
        }
    }
	else if(command == "TurnLeft"){
		turnLeft();
    }
	
}

/*void callback(const boost::shared_ptr<const race::pid_input> &data, ros::Publisher &pub){

	// TEST: show time elapsed between callbacks
	// auto new_start = std::chrono::high_resolution_clock::now();
	// std::chrono::duration<double> elapsed = new_start - end_time;
	// cout << "Elapsed time between callback: " << (double)elapsed.count() << "s"<<endl;

	double angle = data.get()->pid_error*kp;
	race::drive_param msg;

	if (angle > 100){
		angle = 100;
	}	
	if (angle < -100){
		angle = -100;
	}
	
	if(data.get()->pid_vel == 0){
		msg.velocity = -8;
	}
	else{
		msg.velocity = vel_input;
	}	
	msg.angle = angle;

	pub.publish(msg);

	// TEST:
	//end_time = std::chrono::high_resolution_clock::now();
}
*/


void turnLeft(){
	change_speed(0.0);
	sleep(1);
	change_angle(100);
	sleep(1);
	change_speed(12);
	usleep(500);
	change_speed(0);
	sleep(1);
	change_angle(-100);
	sleep(1);
	change_speed(-12);
	usleep(500);
	change_speed(0);
	sleep(1);
	change_angle(0);
}

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


void read_commands(){

while(true) {

	string command;
	string command_s1;
	string command_s2;
	// Get user input
	string delimeter = " ";
	cout << "Listening to error for PID" << "\n";
	cout << "Enter commands: ";
	getline(cin, command);
	vector<string> words{};
	size_t pos = 0;
	cout << command << endl;
	string wholeCommand = command + " ";
	while((pos = wholeCommand.find(delimeter)) != string::npos){
			words.push_back(wholeCommand.substr(0, pos));
			wholeCommand.erase(0, pos + delimeter.length());
	}

	//detect functionn
	if (words[0].empty()){
			cout << "NO ARGS" << endl;
	}
	else if(words[0] == "s"){

		if(words[1].empty()){
				cout<< "NO SPEED ARG" << endl;
		}
		else{	
			double speed = stod(words[1]);	
			cout << speed << endl;
			race::drive_param msg;
			change_speed(speed, pub);
			}

      }
		else if (words[0] == "t"){
			double angle = stod(words[1]);
            race::drive_param msg;
			change_angle(angle, pub);
		}
		else if (words[0] == "c"){
			do_circle(pub);
		}

        else{
            cout << "invalid command" << endl;
        }
        cout << "Processed Command\n";

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
		ros::Publisher pub = p.advertise<race::drive_param>("drive_parameters", pub_queue_size);
		// Create Node Handeler for subscriber
		
		ros::NodeHandle s;
		ros::Subscriber sub = s.subscribe<race::pid_input>("error", sub_queue_size, boost::bind(callback, _1, pub));
		cout<< "intialized nodes\n";

		//listen for commands from camera
		//while(true){
		
	ros::init(argc, argv, "camSub");
        int sub_queue_size = 1;
        ros::NodeHandle s;
        ros::Subscriber sub = s.subscribe<std_msgs::String>("CV_Command", sub_queue_size, callBack);
        //}
		
	}

	return 0;
}
