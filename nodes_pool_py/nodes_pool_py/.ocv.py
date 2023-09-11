#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import cv2

class NodeCam(Node):

    def __init__(self):
        super().__init__("Camera_node")
        self.gqt_logger().info("Камера включена!")
        self.video_stream()

    def video_stream(self):
        self.cap = cv2.VideoCapture(0)
        if (cap.isOpened()):
            try:
                while (cap.isOpened()):
                    ret, frame = cap.read()
                    if not ret:
                        break
                    cv2.imshow('test', frame)
                    if cv2.waitKey(1) & 0xFF == ord('q'):
                        break
            except Exception:
                pass
            finally:
                cap.release()
                cv2.destroyAllWindows()

def main(args=None):
    rclpy.init(args=args)
    node = NodeCam()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()

    