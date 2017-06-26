//
// Author: Ori Novanda (cargmax-at-gmail.com)
//

#include "benchmark.h"
#include "ros/ros.h"
#include "performance_tests/SuperAwesome.h"
#include "performance_tests/SetRate.h"

using namespace std;

RxStats::RxStats() {
	size = 50;
	reset();
}

RxStats::RxStats(int length) {
	size = length;
}

void RxStats::reset() {
	buffer.clear();
	buffer = deque<long>();
	previous = chrono::system_clock::time_point();
	expected = 0;
	first = true;
	lost = 0;
}

void RxStats::receive(const performance_tests::SuperAwesome::ConstPtr& msg) {
	int val = stoi(msg->data);
	int subtotal = 0;
	if(val != expected) {
		if(first) first = false;
		else  {
			subtotal = val - expected; 
			lost += subtotal;
			//ROS_INFO("Message(s) lost (total = %d/%d)! Expected = %d, Received: %d", subtotal, lost, expected, val);
		}
	}
	expected = val+1;

	chrono::system_clock::time_point t = chrono::system_clock::now();
	time_t tt = chrono::system_clock::to_time_t(previous);
	if ((long) tt == 0) {
		previous = t;
		return;
	}
	chrono::system_clock::duration td = t - previous;
	previous = t;
	if(buffer.size() == size) buffer.pop_front();
	buffer.push_back(td.count());
	//ROS_INFO("Delta: %ld", td.count());
}

double RxStats::getMean() {
	if(buffer.size() == 0) return 0;
	lock.lock();
	double sum = 0;
	for(int i=0; i<buffer.size(); i++) {
		sum += buffer.at(i);
	}
	lock.unlock();
	return sum/buffer.size();
}

double RxStats::getHz() {
	double mean = getMean();
	if (mean == 0) return 0;
	else return 1000000000. / mean;
}

int RxStats::getLostCount() {
	return lost;
}	

