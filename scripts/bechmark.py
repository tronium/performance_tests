#!/usr/bin/env python

#
# Author: Ori Novanda (cargmax-at-gmail.com)
#

import threading
from datetime import datetime

class RxStats(object):
	def __init__(self, length=50):
		self.lock = threading.Lock()
		self.size = length
		self.reset()

	def reset(self):
		self.buffer = []
		self.t0 = None
		self.expected = 0
		self.first = True
		self.lost = 0

	def receive(self, data):
		val = int(data);
		if val != self.expected:
			if self.first:
				self.first = False
			else:
				subtotal = val - self.expected
				self.lost += subtotal
				#rospy.loginfo("Message(s) lost (%d/%d)! Expected = %d, Received: %d", subtotal, self.lost, self.expected, val)
		self.expected = val + 1;

		t = datetime.now() 
		if self.t0 is None:
			self.t0 = t
			return
		td = t - self.t0
		self.t0 = t
		with self.lock:
			if(len(self.buffer)==self.size):
				self.buffer.pop(0)
			self.buffer.append(td.microseconds)
		#rospy.loginfo(td.microseconds)

	def getMean(self):
		with self.lock:
			mean = sum(self.buffer) / len(self.buffer) if len(self.buffer) > 0 else 0
			return mean

	def getHz(self):
		mean = self.getMean()
		return 1000000. / mean if mean > 0. else 0

	def getLostCount(self):
		return self.lost

