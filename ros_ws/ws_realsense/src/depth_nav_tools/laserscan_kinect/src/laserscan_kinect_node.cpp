#include <laserscan_kinect/laserscan_kinect_node.h>
#include <boost/bind.hpp>

namespace laserscan_kinect {

//left realsense camera
LaserScanKinectNode::LaserScanKinectNode(ros::NodeHandle& n, ros::NodeHandle& pnh) :
    pnh_(pnh), it_(n), srv_(pnh) {
  std::lock_guard<std::mutex> lock(connect_mutex_);

  // Dynamic reconfigure server callback
  srv_.setCallback(boost::bind(&LaserScanKinectNode::reconfigureCb, this, _1, _2));

  // Subscription to depth image topic
  pub_ = n.advertise<sensor_msgs::LaserScan>("/realsense/scan_left", 10,
                                              boost::bind(&LaserScanKinectNode::connectCb, this, _1),
                                              boost::bind(&LaserScanKinectNode::disconnectCb, this, _1));
}

LaserScanKinectNode::~LaserScanKinectNode() {
  sub_.shutdown();
}

void LaserScanKinectNode::depthCb(const sensor_msgs::ImageConstPtr& depth_msg,
                                  const sensor_msgs::CameraInfoConstPtr& info_msg) {
  try {
    sensor_msgs::LaserScanPtr laserscan_msg = converter_.getLaserScanMsg(depth_msg, info_msg);

    pub_.publish(laserscan_msg);
  }
  catch (std::runtime_error& e) {
    ROS_ERROR_THROTTLE(1.0, "Could not convert depth image to laserscan: %s", e.what());
  }
}

void LaserScanKinectNode::connectCb([[maybe_unused]] const ros::SingleSubscriberPublisher& pub) {
  std::lock_guard<std::mutex> lock(connect_mutex_);

  //if (sub_ != nullptr && pub_.getNumSubscribers() > 0) {
    ROS_DEBUG("Connecting to depth topic.");
    image_transport::TransportHints hints("raw", ros::TransportHints(), pnh_);
    sub_ = it_.subscribeCamera("image_left", 10, &LaserScanKinectNode::depthCb, this, hints);
  //}
}

void LaserScanKinectNode::disconnectCb([[maybe_unused]] const ros::SingleSubscriberPublisher& pub) {
//  std::lock_guard<std::mutex> lock(connect_mutex_);

//  if (pub_.getNumSubscribers() == 0) {
//    ROS_DEBUG("Unsubscribing from depth topic.");
//    sub_.shutdown();
//  }
}

void LaserScanKinectNode::reconfigureCb(laserscan_kinect::LaserscanKinectConfig& config,
                                        [[maybe_unused]] uint32_t level) {
  converter_.setOutputFrame(config.output_frame_id);
  converter_.setRangeLimits(config.range_min, config.range_max);
  converter_.setScanHeight(config.scan_height);
  converter_.setDepthImgRowStep(config.depth_img_row_step);
  converter_.setCamModelUpdate(config.cam_model_update);

  converter_.setSensorMountHeight(config.sensor_mount_height);
  converter_.setSensorTiltAngle(config.sensor_tilt_angle);
  converter_.setGroundRemove(config.ground_remove_en);
  converter_.setGroundMargin(config.ground_margin);
  converter_.setTiltCompensation(config.tilt_compensation_en);

  converter_.setScanConfigurated(false);
}


//right realsense camera


LaserScanRealsenseNode::LaserScanRealsenseNode(ros::NodeHandle& n, ros::NodeHandle& pnh) :
    pnh_(pnh), it_(n), srv_(pnh) {
  std::lock_guard<std::mutex> lock(connect_mutex_);

  // Dynamic reconfigure server callback
  srv_.setCallback(boost::bind(&LaserScanRealsenseNode::reconfigureCb, this, _1, _2));

  // Subscription to depth image topic
  pub_ = n.advertise<sensor_msgs::LaserScan>("/realsense/scan_right", 10,
                                              boost::bind(&LaserScanRealsenseNode::connectCb, this, _1),
                                              boost::bind(&LaserScanRealsenseNode::disconnectCb, this, _1));
}

LaserScanRealsenseNode::~LaserScanRealsenseNode() {
  sub_.shutdown();
}

void LaserScanRealsenseNode::depthCb(const sensor_msgs::ImageConstPtr& depth_msg,
                                  const sensor_msgs::CameraInfoConstPtr& info_msg) {
  try {
    sensor_msgs::LaserScanPtr laserscan_msg = converter_.getLaserScanMsg(depth_msg, info_msg);

    pub_.publish(laserscan_msg);
  }
  catch (std::runtime_error& e) {
    ROS_ERROR_THROTTLE(1.0, "Could not convert depth image to laserscan: %s", e.what());
  }
}

void LaserScanRealsenseNode::connectCb([[maybe_unused]] const ros::SingleSubscriberPublisher& pub) {
  std::lock_guard<std::mutex> lock(connect_mutex_);

  //if (sub_ != nullptr && pub_.getNumSubscribers() > 0) {
    ROS_DEBUG("Connecting to depth topic.");
    image_transport::TransportHints hints("raw", ros::TransportHints(), pnh_);
    sub_ = it_.subscribeCamera("image_right", 10, &LaserScanRealsenseNode::depthCb, this, hints);
  //}
}

void LaserScanRealsenseNode::disconnectCb([[maybe_unused]] const ros::SingleSubscriberPublisher& pub) {
//  std::lock_guard<std::mutex> lock(connect_mutex_);

//  if (pub_.getNumSubscribers() == 0) {
//    ROS_DEBUG("Unsubscribing from depth topic.");
//    sub_.shutdown();
//  }
}

void LaserScanRealsenseNode::reconfigureCb(laserscan_kinect::LaserscanKinectConfig& config,
                                        [[maybe_unused]] uint32_t level) {
  converter_.setOutputFrame(config.output_frame_id);
  converter_.setRangeLimits(config.range_min, config.range_max);
  converter_.setScanHeight(config.scan_height);
  converter_.setDepthImgRowStep(config.depth_img_row_step);
  converter_.setCamModelUpdate(config.cam_model_update);

  converter_.setSensorMountHeight(config.sensor_mount_height);
  converter_.setSensorTiltAngle(config.sensor_tilt_angle);
  converter_.setGroundRemove(config.ground_remove_en);
  converter_.setGroundMargin(config.ground_margin);
  converter_.setTiltCompensation(config.tilt_compensation_en);

  converter_.setScanConfigurated(false);
}



























} // namespace laserscan_kinect
