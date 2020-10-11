#!/usr/bin/env python

import rospy

from dynamic_reconfigure.server import Server
from pcl_detector.cfg import cloudConfig

def callback(config, level):
    rospy.loginfo("""Reconfigure Request: {kmean}, {threshold}\ 
          """.format(**config))
    return config

if __name__ == "__main__":
    rospy.init_node("pcl_detector", anonymous = False)

    srv = Server(cloudConfig, callback)
    rospy.spin()