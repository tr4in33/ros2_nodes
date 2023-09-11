#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
import cv2
from cv_bridge import CvBridge
import numpy as np
import time


class MinimalSubscriber(Node):

    def __init__(self):
        super().__init__('minimal_subscriber')
        self.frames, self.loop_time = 0, time.time()
        self.prevframetime, self.newframetime = 0, 0
        self.subscription = self.create_subscription(
            Image,
            'flycam/image_raw',
            #'flycam/image_rect',
            #'image',
            self.listener_callback,
            10)
        self.subscription  # prevent unused variable warning
        self.bridge = CvBridge()

    def listener_callback(self, msg):
        bridge  = self.bridge
        cv_img = bridge.imgmsg_to_cv2(msg, desired_encoding='passthrough')
        self.frames += 1
        self.newframetime = time.time()
        showfps = int (1 /(self.newframetime-self.prevframetime))
        self.prevframetime = self.newframetime
        showfps = str("{} fps".format(showfps))
        print (showfps)
        # self.get_logger().info('I heard: "%s"' % msg.data)
        
        #img = np.reshape(msg.data, (msg.height, msg.width))
        #img = msg.data
        
        # cv2.imshow('d', cv_img)
        # cv2.waitKey()
        # print(np.array(img).shape)
        #print(np.array(img))


def main(args=None):
    rclpy.init(args=args)

    minimal_subscriber = MinimalSubscriber()

    rclpy.spin(minimal_subscriber)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()
