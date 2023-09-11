#pragma once

#include <string>

#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/image_encodings.hpp>
#include <sensor_msgs/distortion_models.hpp>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/videoio.hpp>

namespace flycam
{
class FlyCam : public rclcpp::Node
{
public:
    FlyCam();

private:


    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr ImgPub{nullptr};
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr RectImgPub{nullptr};
    rclcpp::Publisher<sensor_msgs::msg::CameraInfo>::SharedPtr InfoPub{nullptr};

    rclcpp::TimerBase::SharedPtr timer;

    void resetCamera();
    void updateHandler();

    sensor_msgs::msg::CameraInfo generateCameraInfo(const bool Camera);

    double fov;
    double fps;
    int cameraIndex;
    bool undirstortRectify;
    bool monochrome;

    cv::VideoCapture vCapture;
    cv::Mat latestFrame, Img;
    

    cv_bridge::CvImage CvBridge;
    sensor_msgs::msg::Image Msg;
    //sensor_msgs::msg::CameraInfo Info;
    
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_publisher;
};
}