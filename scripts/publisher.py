#!/usr/bin/env python

#
# Author: Ori Novanda (cargmax-at-gmail.com)
#

import rospy, time
from performance_tests.msg import SuperAwesome
from performance_tests.srv import *

sendRate = 10
rate = None

def setRate_Handler(req):
	global rate, sendRate
	sendRate = req.rate
	if sendRate != 0:
		rate = rospy.Rate(sendRate)
		rospy.loginfo("Publish rate: %f", req.rate)
	else:
		rospy.loginfo("Pause sending")
	return SetRateResponse()

def publisher():
	global rate
	pub = rospy.Publisher("benchmark", SuperAwesome, queue_size=100)
	rospy.init_node("python_publisher", anonymous=True)
	s = rospy.Service("SetPublishRate", SetRate, setRate_Handler)
	rate = rospy.Rate(sendRate)
	count = 1
	msg = SuperAwesome()
	while not rospy.is_shutdown():
		if rate == 0:
			time.sleep(0.1)
			next
		msg.data = "%d" % count;
		#rospy.loginfo(msg)
		pub.publish(msg)
		count += 1
		if sendRate == 0:
			next
		rate.sleep()

if __name__ == "__main__":
	try:
		publisher()
	except rospy.ROSInterruptException:
		pass

