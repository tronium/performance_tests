//
// Author: Ori Novanda (cargmax-at-gmail.com)
//

#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#include "performance_tests/SuperAwesome.h"
#include <chrono>
#include <mutex>
#include <deque>
#include <thread>

using namespace std;

class RxStats {
  public:
	RxStats();
	RxStats(int length);
	void reset();
	void receive(const performance_tests::SuperAwesome::ConstPtr& msg);
	double getMean();
	double getHz();
	int getLostCount();
  private:
	deque<long> buffer;
	mutex lock;
	int size;
	chrono::system_clock::time_point previous;
	int expected;
	bool first;
	int lost;
};

#endif // __BENCHMARK_H__
