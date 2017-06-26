#!/usr/bin/env python

#
# Author: Ori Novanda (cargmax-at-gmail.com)
#

import rospy, threading, time
from performance_tests.msg import SuperAwesome
from performance_tests.srv import *
from bechmark import RxStats

svc = None
ts = RxStats()

def callback(data):
	#rospy.loginfo("Received: %s", data.data)
	ts.receive(data.data)

def subcribe():
	global svc
	rospy.init_node("python_subscriber", anonymous=True)
	rospy.Subscriber("benchmark", SuperAwesome, callback, queue_size=100)
	th = threading.Thread(target=worker, args=("Worker", ts))
	th.start()
	rospy.spin()
	th.join()

def worker(tname, ts):
	rospy.wait_for_service("SetPublishRate")
	svc = rospy.ServiceProxy("SetPublishRate", SetRate)
	duration = 5
	speeds = [10, 50, 100, 250, 500, 750, 1000, 1250, 1500, 1750, 2000, 3000, 4000, 5000, 6000]
	#speeds[] = [6000, 5000, 4000, 3000, 2000, 1750, 1500, 1250, 1000, 750, 500, 250, 100, 50, 10]
	svc(0)
	for speed in speeds:
		ts.reset()
		rospy.loginfo("Preparing benchmark at %d Hz rate", speed)
		time.sleep(1)
		rospy.loginfo("Processing (for %d seconds)", duration)
		svc(speed)
		time.sleep(duration)
		svc(0)
		time.sleep(1)
		rospy.loginfo("Result:")
		rospy.loginfo("Requested = %d Hz, Received = %lf Hz, Lost message(s) = %d", speed, ts.getHz(), ts.getLostCount())
	
	rospy.signal_shutdown("Done")

if __name__ == "__main__":
	subcribe()

