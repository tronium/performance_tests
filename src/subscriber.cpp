//
// Author: Ori Novanda (cargmax-at-gmail.com)
//

#include "benchmark.h"
#include "ros/ros.h"
#include "performance_tests/SuperAwesome.h"
#include "performance_tests/SetRate.h"

ros::ServiceClient client;
performance_tests::SetRate svc;
RxStats ts;
bool exitFlag = false;

void subscriberCallback(const performance_tests::SuperAwesome::ConstPtr& msg) {
	//ROS_INFO("Received: %s", msg->data.c_str());
	ts.receive(msg);
}

void worker() {
	int duration = 5;
	int speeds[] = {10, 50, 100, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 3000, 4000, 5000, 6000};
	//int speeds[] = {6000, 5000, 4000, 3000, 2000, 1750, 1500, 1250, 1000, 750, 500, 250, 100, 50, 10};
	svc.request.rate = 0;
	client.call(svc);
	for (int speed : speeds) {
		ROS_INFO("Preparing benchmark at %d Hz rate", speed);
		ros::Duration(1).sleep();
		ts.reset();
		ROS_INFO("Processing (for %d seconds)", duration);
		svc.request.rate = speed;
		client.call(svc);
		ros::Duration(duration).sleep();
		svc.request.rate = 0;
		client.call(svc);
		ros::Duration(1).sleep();
		ROS_INFO("Result:");
		ROS_INFO("Requested = %d Hz, Received = %lf Hz, Lost message(s) = %d", speed, ts.getHz(), ts.getLostCount());
	}
	ros::shutdown();
}

//void exit_handler() {
//}

int main(int argc, char **argv) {
	//atexit(exit_handler);
	ros::init(argc, argv, "c_subscriber");
	ros::NodeHandle nh;
	client = nh.serviceClient<performance_tests::SetRate>("SetPublishRate", true);
	
	ros::Subscriber sub = nh.subscribe("benchmark", 100, subscriberCallback);
	thread t1(worker);
	ros::spin();
	t1.join();
	//cout << "DONE\n";	
	return 0;
}

