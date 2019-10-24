// ROS
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/CameraInfo.h>
#include <cv_bridge/cv_bridge.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>

// openCV
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

// Topics for the data
static string topicInImage, topicOutOdom, topicOutPixel, topicCameraInfo;
static string parentFrameId;

// Subs - Pubs
static ros::Subscriber imageSub;
static ros::Publisher imagePub;
static ros::Subscriber cameraInfoSub;
static ros::Publisher cameraInfoPub;

// Image
static cv::Mat image;
static std_msgs::Header imageHeader;

// CameraParameter
static sensor_msgs::CameraInfo cameraInfo;
static volatile bool setCameraInfo = false;

// Parameters
static int verbose = 0;
static int gui, mono2bayer, bgr2mono, rgb2mono, forwardImage, gaussianDemosaic;
static std::string windowName;

// Demosaic code from https://github.com/opencv/opencv/blob/master/modules/imgproc/include/opencv2/imgproc.hpp
static int demosaicCode = 0;

// Prototypes
void callbackCameraInfo(sensor_msgs::CameraInfo msg);
void callbackImage(sensor_msgs::ImageConstPtr msg);
static void programOptions(ros::NodeHandle &n);

static void programOptions(ros::NodeHandle &n) {
  n.param<std::string>("topic_in_image", topicInImage, "/genicam/cam4"); // Video parameter for the camera
  n.param<std::string>("topic_camera_info", topicCameraInfo, "/genicam/camera_info"); // Camera info for the image
  n.param<std::string>("topic_out_odom", topicOutOdom, "/odom"); // scope for sending the odometries
  n.param<std::string>("topic_out_pixel", topicOutPixel, "/pixel"); // scope for sending the pixel data
  n.param<int>("demosaic_code", demosaicCode, 0); // Code for demosaicing
  n.param<int>("gui", gui, 0); // Show image by OpenCV
  n.param<int>("forward_image", forwardImage, 0); // Forwards the image without processin
  n.param<int>("mono2bayer", mono2bayer, 0); // Convert a mono image to bayer pattern
  n.param<int>("gaussian_demosaic", gaussianDemosaic, 0); // Using Gauss kernel for demosaicing
  n.param<int>("rgb2mono", rgb2mono, 0); // Convert the debayered image to mono again
  n.param<int>("bgr2mono", bgr2mono, 0); // Convert the debayered image to mono again
  n.param<std::string>("window_name", windowName, ros::this_node::getName()); // Window Name
  n.param<int>("verbose", verbose, 0); // Verbosity
}

void callbackCameraInfo(sensor_msgs::CameraInfo msg) {
  if (cameraInfo.K != msg.K) { // if something changed
    cameraInfo = msg;
    ROS_INFO("[%s] Update camera_info.", ros::this_node::getName().c_str());
    setCameraInfo = true;
  }
}

void callbackImage(sensor_msgs::ImageConstPtr msg) {
  if (!setCameraInfo) {
    ROS_WARN("[%s] Abort, no cameraInfo recieved yet.", ros::this_node::getName().c_str());
    return;
  }

  if (msg->data.size() != msg->height * msg->width) {
    ROS_ERROR("[%s] Abort, image is not single channeled.", ros::this_node::getName().c_str());
    return;
  }

  // Get reference
  image = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8)->image;

  if (!forwardImage) {
    if (mono2bayer) {
      // Convert to bayer pattern, so that OpenCV produces the correct image output
      // See color scaling in https://docs.opencv.org/3.1.0/de/d25/imgproc_color_conversions.html
      // cv::cvtColor(cv_bridge::toCvShare(msg, msg->encoding)->image, image, cv::COLOR_GRAY2BGR);
      // image = cv_bridge::toCvShare(msg, msg->encoding)->image;
    }

    if (gaussianDemosaic) {
      cv::GaussianBlur(image, image, cv::Size(3, 3), 0, 0, cv::BORDER_DEFAULT);
    } else {
      cv::cvtColor(image, image, demosaicCode);
    }

    if (image.channels() == 3) {
      if (rgb2mono) {
        cvtColor(image, image, cv::COLOR_RGB2GRAY);
      } else if (bgr2mono) {
        cvtColor(image, image, cv::COLOR_BGR2GRAY);
      }
    } else if (image.channels() == 1) {
      // Nothing to do
    } else {
      ROS_WARN("[%s] Abort, undefined behaviour.", ros::this_node::getName().c_str());
      return;
    }
  }

  // Prepare the image message
  sensor_msgs::Image msgImage;
  if (image.channels() == 3) {
    msgImage.encoding = sensor_msgs::image_encodings::RGB8;
    // msgImage.encoding = sensor_msgs::image_encodings::BGR8;
  } else { // (frameDst->channels() == 1)
    msgImage.encoding = sensor_msgs::image_encodings::MONO8;

    msgImage.height = image.rows;
    msgImage.width = image.cols;
    msgImage.is_bigendian = false;
    msgImage.header.stamp = msg->header.stamp;
    cameraInfo.header.stamp = msg->header.stamp;
    msgImage.data.resize(image.rows * image.cols * image.channels());
    msgImage.step = image.cols * image.channels();
    memcpy(msgImage.data.data(), image.data, msgImage.data.size());

    // Publish
    imagePub.publish(msgImage);
    cameraInfoPub.publish(cameraInfo);
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, ros::this_node::getName());
  ros::NodeHandle n("~");
  programOptions(n);

  imageSub = n.subscribe(topicInImage, 1, callbackImage);
  imagePub = n.advertise<sensor_msgs::Image>("image_raw", 2);
  cameraInfoSub = n.subscribe(topicCameraInfo, 1, callbackCameraInfo);
  cameraInfoPub = n.advertise<sensor_msgs::CameraInfo>("camera_info", 1);

  if (gui) {
    cv::namedWindow(windowName, CV_WINDOW_FREERATIO);
  }

  ros::spin();
}
