#include "airplane_ros/airplane_cam_cpp.hpp"


#include <geometry_msgs/msg/transform_stamped.hpp>

namespace flycam
{

uint32_t height = 720;
uint32_t width = 1280;

using namespace std::chrono_literals;

FlyCam::FlyCam() : Node("flycam") 
{
    // calibFile = declare_parameter("calibration_file", "");
    fov = declare_parameter("fov", 100.0);
    fps = declare_parameter("fps", 30.0);
    undirstortRectify = declare_parameter("undistort_rectify", false);
    cameraIndex = declare_parameter("camera_index", 3);
    monochrome = declare_parameter("monochrome", false);
 
    cv::setNumThreads(4);

    // if(calibFile.empty())
    // {
    //     RCLCPP_FATAL(get_logger(), "CALIBRATION FILE PATH IS EMPTY");
    //     exit(-1);
    // }
    // try
    // {
    //     calib = Calibration(calibFile);
    // }
    // catch(const std::runtime_error& e)
    // {
    //     RCLCPP_FATAL(get_logger(), e.what());
    //     exit(-1);
    // }
    
    //Info = generateCameraInfo(true);
   

    CvBridge.encoding = monochrome ? sensor_msgs::image_encodings::TYPE_8UC1 : sensor_msgs::image_encodings::TYPE_8UC3;
    

    CvBridge.header.frame_id = std::string(this->get_name());


    // publish static transform between sensor frames
    tf_publisher = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
    
    geometry_msgs::msg::TransformStamped Tf;
    Tf.header.frame_id = this->get_name();
    Tf.child_frame_id = CvBridge.header.frame_id;
    Tf.header.stamp = now();
    Tf.transform.rotation.x = 0.5;
    Tf.transform.rotation.y = -0.5;
    Tf.transform.rotation.z = 0.5;
    Tf.transform.rotation.w = -0.5;
    tf_publisher->sendTransform(Tf);

    ImgPub = create_publisher<sensor_msgs::msg::Image>(std::string(this->get_name()) + "/image_raw", 10);
    RectImgPub = create_publisher<sensor_msgs::msg::Image>(std::string(this->get_name()) + "/image_rect", 10);
    //InfoPub = create_publisher<sensor_msgs::msg::CameraInfo>(std::string(this->get_name()) + "/camera_info", 10);


    vCapture.open(cameraIndex,  cv::CAP_ANY);
    vCapture.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH,  width);
    vCapture.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT, height);
    vCapture.set(cv::VideoCaptureProperties::CAP_PROP_FPS, fps);

    timer = create_wall_timer(std::chrono::milliseconds(static_cast<int>(1000 / fps)), std::bind(&FlyCam::updateHandler, this));
    RCLCPP_INFO(get_logger(), "Starting camera");
}

sensor_msgs::msg::CameraInfo FlyCam::generateCameraInfo(const bool Camera)
{
    sensor_msgs::msg::CameraInfo info;
    info.height = 720;
    info.width = 1280;
    info.distortion_model = sensor_msgs::distortion_models::RATIONAL_POLYNOMIAL;

    cv::Size size;

    // const cv::Mat K = Camera ? calib.Kl : calib.Kr;
    // size = K.size();
    // for(int i = 0; i < size.height; i++)
    // {
    //     for(int j = 0; j < size.width; j++)
    //     {
    //         info.k[i * 3 + j] = K.at<double>(i, j);
    //     }
    // }

    // const cv::Mat R = Camera ? calib.Rl : calib.Rr;
    // size = R.size();
    // for(int i = 0; i < size.height; i++)
    // {
    //     for(int j = 0; j < size.width; j++)
    //     {
    //         info.r[i * 3 + j] = R.at<double>(i, j);
    //     }
    // }

    // const cv::Mat D = Camera ? calib.Dl : calib.Dr;
    size = D.size();
    for(int i = 0; i < size.height; i++)
    {
        for(int j = 0; j < size.width; j++)
        {
            info.d.push_back(D.at<double>(i, j));
        }
    }

    // if(!Camera)
    // {
    //     // Tx = -fx * camera_baseline (the baselink in the calibration is already negative)
    //     info.p[3] = info.p[0] * calib.Translation.at<double>(0, 0);
    // }

    return info;
}

  
void FlyCam::resetCamera()
{

    vCapture.release();
    vCapture.open(cameraIndex);
    vCapture.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_WIDTH,  width);
    vCapture.set(cv::VideoCaptureProperties::CAP_PROP_FRAME_HEIGHT, height);
    vCapture.set(cv::VideoCaptureProperties::CAP_PROP_FPS, fps);
}

void FlyCam::updateHandler()
{


    if(!vCapture.isOpened())
    {
        RCLCPP_ERROR(get_logger(), "CAN'T CONNECTO TO CAMERA %i", cameraIndex);
        resetCamera();
        return;
    }

    vCapture >> latestFrame;
    CvBridge.header.stamp = now();
   

    if(latestFrame.total() == 0)
    {
        RCLCPP_WARN(get_logger(), "RECEIVED EMPTY IMAGE");
        resetCamera();
        return;
    }

    if(monochrome) cv::cvtColor(latestFrame, latestFrame, cv::COLOR_BGR2GRAY);

    
    latestFrame(cv::Rect(0, 0, width, height)).copyTo(CvBridge.image);
    

    CvBridge.toImageMsg(Msg);
    

    //Info.header = Msg.header;
  

    //InfoPub->publish(Info);
    


    ImgPub->publish(Msg);
    

}
}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<flycam::FlyCam>());
  rclcpp::shutdown();
  return 0;
}