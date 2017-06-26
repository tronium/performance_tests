//
// Author: Ori Novanda (cargmax-at-gmail.com)
//

#include "ros/ros.h"
#include "performance_tests/SuperAwesome.h"
#include "performance_tests/SetRate.h"
#include <sstream>

double sendRate = 10;
bool sendRateUpdated = false;

bool setRate(performance_tests::SetRate::Request &req,
		performance_tests::SetRate::Response &res) {
	sendRate = req.rate;
	if(sendRate != 0) {
		//rate = ros::Rate(sendRate);
		ROS_INFO("Publish rate: %lf", sendRate);
		sendRateUpdated = true;
	}
	else {
		ROS_INFO("Pause sending");
	}
	return true;	
}
		
int main(int argc, char **argv) {
	ros::Time::init();
	ros::init(argc, argv, "c_publisher");
	ros::NodeHandle nh;
        ros::Publisher pub = nh.advertise<performance_tests::SuperAwesome>("benchmark",100);
	ros::ServiceServer svc =  nh.advertiseService("SetPublishRate", setRate);
	
	ros::Rate rate = ros::Rate(sendRate);

	int count = 1;
	performance_tests::SuperAwesome msg;
	while (ros::ok()) {
		if(sendRate == 0) {
			ros::spinOnce();
			ros::Duration(0.1).sleep();
			continue;
		}
		else if(sendRateUpdated) {
			rate = ros::Rate(sendRate);
			sendRateUpdated = false;
		}
		std::stringstream ss;
		ss << count++;
		msg.data = ss.str();
		//ROS_INFO("%s", msg.data.c_str());
		pub.publish(msg);
		ros::spinOnce();
		if(sendRate == 0) continue;
		rate.sleep();
	}
	return 0;
}

