// Copyright (c) 2022, CHRISLab, Christopher Newport University
// Copyright (c) 2008, Willow Garage, Inc.
// All rights reserved.
//
// Software License Agreement (BSD License 2.0)
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//  * Neither the name of the Willow Garage nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef IMAGE_FLIP__IMAGE_FLIP_NODE_HPP_
#define IMAGE_FLIP__IMAGE_FLIP_NODE_HPP_

#include <math.h>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/static_transform_broadcaster.h>

#include <memory>
#include <string>

#include <cv_bridge/cv_bridge.hpp>
#include <image_transport/image_transport.hpp>
#include <rclcpp/rclcpp.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>

#include "image_flip/visibility.h"

namespace image_flip
{

struct ImageFlipConfig
{
  std::string output_frame_id;
  int rotation_steps;
  bool use_camera_info;
  rmw_qos_profile_t input_qos;  // "default", "sensor_data", etc ..
  rmw_qos_profile_t output_qos;
};

class ImageFlipNode : public rclcpp::Node
{
public:
  IMAGE_FLIP_PUBLIC ImageFlipNode(rclcpp::NodeOptions options);

private:
  const std::string frameWithDefault(const std::string & frame, const std::string & image_frame);
  void imageCallbackWithInfo(
    const sensor_msgs::msg::Image::ConstSharedPtr & msg,
    const sensor_msgs::msg::CameraInfo::ConstSharedPtr & cam_info);
  void imageCallback(const sensor_msgs::msg::Image::ConstSharedPtr & msg);
  void do_work(
    const sensor_msgs::msg::Image::ConstSharedPtr & msg,
    const sensor_msgs::msg::CameraInfo::ConstSharedPtr & cam_info,
    const std::string input_frame_from_msg);
  void subscribe();
  void unsubscribe();
  void connectCb();
  void disconnectCb();
  void onInit();

  rclcpp::Node::OnSetParametersCallbackHandle::SharedPtr on_set_parameters_callback_handle_;

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_sub_;
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_pub_;
  bool tf_unpublished_;

  image_flip::ImageFlipConfig config_;

  image_transport::Publisher img_pub_;
  image_transport::Subscriber img_sub_;
  image_transport::CameraSubscriber cam_sub_;
  image_transport::CameraPublisher cam_pub_;

  int subscriber_count_;
  double angle_;
  tf2::TimePoint prev_stamp_;
  geometry_msgs::msg::TransformStamped transform_;
};
}  // namespace image_flip

#endif  // IMAGE_FLIP__IMAGE_FLIP_NODE_HPP_
