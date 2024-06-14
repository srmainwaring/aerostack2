// Copyright 2023 Universidad Politécnica de Madrid
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//    * Neither the name of the Universidad Politécnica de Madrid nor the names of its
//      contributors may be used to endorse or promote products derived from
//      this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/*!*******************************************************************************************
 *  \file       sensor.hpp
 *  \brief      Sensor class for AS2 header file
 *  \authors    Rafael Pérez Seguí
 *              David Pérez Saura
 *              Miguel Fernández Cortizas
 *              Pedro Arias Pérez
 ********************************************************************************/

#ifndef AS2_CORE__SENSOR_HPP_
#define AS2_CORE__SENSOR_HPP_

#include <cv_bridge/cv_bridge.hpp>
#include <tf2_ros/static_transform_broadcaster.h>
#include <tf2_ros/transform_broadcaster.h>
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/publisher_options.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/battery_state.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/fluid_pressure.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/magnetic_field.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <sensor_msgs/msg/range.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist_stamped.hpp>
#include <geometry_msgs/msg/quaternion_stamped.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include "as2_core/custom/tf2_geometry_msgs.hpp"
#include "as2_core/names/topics.hpp"
#include "as2_core/node.hpp"
#include "as2_core/utils/tf_utils.hpp"

namespace as2
{
namespace sensors
{

/**
 * @brief TFStatic object to publish static transforms in TF
 *
*/
class TFStatic
{
public:
  /**
   * @brief Construct a new TFStatic object
   *
  */
  explicit TFStatic(rclcpp::Node * node_ptr);

  /**
   * @brief Destroy the TFStatic object
   *
  */
  virtual ~TFStatic();

  /**
   * @brief Set the Static Transform in TF
   *
   * @param transformStamped TransformStamped message
  */
  virtual void setStaticTransform(
    const geometry_msgs::msg::TransformStamped & transformStamped);

  /**
   * @brief Set the Static Transform in TF
   *
   * @param frame_id Frame ID
   * @param parent_frame_id Parent Frame ID
   * @param x X position (m)
   * @param y Y position (m)
   * @param z Z position (m)
   * @param qx Quaternion X
   * @param qy Quaternion Y
   * @param qz Quaternion Z
   * @param qw Quaternion W
  */
  virtual void setStaticTransform(
    const std::string & frame_id, const std::string & parent_frame_id, float x, float y, float z,
    float qx, float qy, float qz, float qw);

  /**
   * @brief Set the Static Transform in TF
   *
   * @param frame_id Frame ID
   * @param parent_frame_id Parent Frame ID
   * @param x X position (m)
   * @param y Y position (m)
   * @param z Z position (m)
   * @param roll Roll (rad)
   * @param pitch Pitch (rad)
   * @param yaw Yaw (rad)
  */
  virtual void setStaticTransform(
    const std::string & frame_id, const std::string & parent_frame_id, float x, float y, float z,
    float roll, float pitch, float yaw);

  /**
   * @brief Get the Node Pointer object
   * @return rclcpp::Node* Node pointer
  */
  const rclcpp::Node * getNode() const;

private:
  rclcpp::Node * node_ptr_ = nullptr;

  /**
   * @brief Set the Static Transform in TF
   *
   * @param transformStamped TransformStamped message
  */
  virtual void setStaticTransform_(const geometry_msgs::msg::TransformStamped & transformStamped);
};  // class TFStatic

/**
 * @brief TFDynamic object to publish dynamic transforms in TF
*/
class TFDynamic
{
public:
  /**
   * @brief Construct a new TFDynamic object
   *
   * @param node_ptr Pointer to the node
  */
  explicit TFDynamic(rclcpp::Node * node_ptr);

  /**
   * @brief Destroy the TFDynamic object
   *
  */
  virtual ~TFDynamic();

  /**
   * @brief Publish the transform
   *
   * @param transform_msg Transform message
  */
  virtual void setDynamicTransform(const geometry_msgs::msg::TransformStamped & transform);

  /**
   * @brief Add a dynamic transform
   *
   * @param frame_id Frame ID
   * @param parent_frame_id Parent Frame ID
   * @param x X position (m)
   * @param y Y position (m)
   * @param z Z position (m)
   * @param qx Quaternion X
   * @param qy Quaternion Y
   * @param qz Quaternion Z
   * @param qw Quaternion W
  */
  virtual void setDynamicTransform(
    const std::string & frame_id, const std::string & parent_frame_id, float x, float y, float z,
    float qx, float qy, float qz, float qw);

  /**
   * @brief Add a dynamic transform
   *
   * @param frame_id Frame ID
   * @param parent_frame_id Parent Frame ID
   * @param x X position (m)
   * @param y Y position (m)
   * @param z Z position (m)
   * @param roll Roll (rad)
   * @param pitch Pitch (rad)
   * @param yaw Yaw (rad)
  */
  virtual void setDynamicTransform(
    const std::string & frame_id, const std::string & parent_frame_id, float x, float y, float z,
    float roll, float pitch, float yaw);

  /**
   * @brief Get the Transform Broadcaster object
   *
   * @return std::shared_ptr<tf2_ros::TransformBroadcaster>
  */
  std::shared_ptr<tf2_ros::TransformBroadcaster> getTransformBroadcaster() const;

  /**
   * @brief Get the Node Pointer object
   * @return rclcpp::Node* Node pointer
  */
  const rclcpp::Node * getNode() const;

protected:
  rclcpp::Node * node_ptr_ = nullptr;
  std::shared_ptr<tf2_ros::TransformBroadcaster> dynamic_tf_broadcaster_ptr_;
};  // class TFDynamic

/**
 * @brief SensorData object to publish data in a topic
 *
 * @tparam T Type of the message
*/
template<typename T>
class SensorData
{
public:
  /**
   * @brief Construct a new GenericSensor object
   *
   * @param node_ptr Pointer to the node
  */
  explicit SensorData(
    const std::string & topic_name, rclcpp::Node * node_ptr,
    bool add_sensor_measurements_base = true)
  {
    // check if topic already has "sensor_measurements "in the name
    // if not, add it if flag add_sensor_measurements_base is true
    topic_name_ = processTopicName(topic_name, add_sensor_measurements_base);

    sensor_publisher_ = node_ptr->create_publisher<T>(
      topic_name_, as2_names::topics::sensor_measurements::qos);
  }

  /**
   * @brief Destroy the GenericSensor object
   *
  */
  virtual ~SensorData() {}

  /**
   * @brief Process the topic name with the sensor measurements base
   *
   * @param topic_name Topic name
   * @param add_sensor_measurements_base Add "sensor_measurements" to the topic name
   * @return std::string Processed topic name
  */
  static std::string processTopicName(
    const std::string & topic_name,
    bool add_sensor_measurements_base = true)
  {
    std::string topic = topic_name;
    if (add_sensor_measurements_base &&
      topic_name.find(as2_names::topics::sensor_measurements::base) == std::string::npos)
    {
      topic = as2_names::topics::sensor_measurements::base + topic_name;
    }
    return topic;
  }

  /**
   * @brief Update the message value
   *
   * @param msg Message
  */
  void setData(const T & msg)
  {
    msg_data_ = msg;
  }

  /**
   * @brief Publish the data stored in the message
   *
  */
  void publish()
  {
    sensor_publisher_->publish(msg_data_);
  }

  /**
   * @brief Update the message value and publish it
   *
   * @param msg Message
  */
  void updateAndPublish(const T & msg)
  {
    msg_data_ = msg;
    publish();
  }

  /**
   * @brief Get the Topic Name object
   * @return std::string Topic name
  */
  const std::string & getTopicName() const
  {
    return topic_name_;
  }

  /**
   * @brief Get the data stored in the message
   * @return T Message
  */
  const T & getData() const
  {
    return msg_data_;
  }

private:
  typename rclcpp::Publisher<T>::SharedPtr sensor_publisher_;
  T msg_data_;
  std::string topic_name_;
};  // class SensorData

/**
 * @brief GenericSensor object to publish sensor data at a given frequency
*/
class GenericSensor
{
public:
  /**
   * @brief Construct a new GenericSensor object
   *
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
  */
  explicit GenericSensor(as2::Node * node_ptr, const float pub_freq = -1.0f);

  /**
   * @brief Destroy the GenericSensor object
  */
  virtual ~GenericSensor();

  /**
   * @brief User must call this function when the data is updated
  */
  void dataUpdated();

  /**
   * @brief User must implement the data publishing in this function
  */
  virtual void publishData() = 0;

private:
  rclcpp::TimerBase::SharedPtr timer_;
  float pub_freq_;

  /**
   * @brief Callback function for the timer. Publishes the data
  */
  void timerCallback();
};

/**
 * @brief Sensor handler to publish sensor data at a given frequency
*/
template<typename T>
class Sensor : public TFStatic, protected GenericSensor, protected SensorData<T>
{
public:
  /**
   * @brief Construct a new Sensor object
   *
   * @param id Sensor ID
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
   * @param add_sensor_measurements_base Add "sensor_measurements" to the topic name
  */
  Sensor(
    const std::string & id, as2::Node * node_ptr, float pub_freq = -1.0f,
    bool add_sensor_measurements_base = true)
  : TFStatic(node_ptr),
    GenericSensor(node_ptr, pub_freq),
    SensorData<T>(id, node_ptr, add_sensor_measurements_base)
  {}

  /**
   * @brief Construct a new Sensor object
   *
   * @param id Sensor ID
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
   * @param add_sensor_measurements_base Add "sensor_measurements" to the topic name
  */
  Sensor(
    const std::string & id, as2::Node * node_ptr, int pub_freq,
    bool add_sensor_measurements_base = true)
  : Sensor(id, node_ptr, static_cast<float>(pub_freq), add_sensor_measurements_base)
  {}

  /**
   * @brief Construct a new Sensor object
   *
   * @param id Sensor ID
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
   * @param add_sensor_measurements_base Add "sensor_measurements" to the topic name
  */
  Sensor(
    const std::string & id, as2::Node * node_ptr, double pub_freq,
    bool add_sensor_measurements_base = true)
  : Sensor(id, node_ptr, static_cast<float>(pub_freq), add_sensor_measurements_base)
  {}

  /**
   * @brief Destroy the Sensor object
   *
  */
  virtual ~Sensor() {}

  /**
   * @brief Update the data of the sensor
   *
   * If the frequency is set to -1, the data is published immediately
   * Otherwise, the data is stored and published at the given frequency
   *
   * @param msg Message
  */
  void updateData(const T & msg)
  {
    SensorData<T>::setData(msg);
    dataUpdated();
  }

protected:
  /**
   * @brief Publish the data in a topic
  */
  void publishData() override
  {
    SensorData<T>::publish();
  }
};  // class Sensor

/**
 * @brief Class to handle the camera sensor
*/
class Camera : public TFStatic, protected GenericSensor
{
public:
  /**
   * @brief Construct a new Camera object
   *
   * @param id Camera ID
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
   * @param add_sensor_measurements_base Add "sensor_measurements" to the topic name
   * @param info_name Name of the camera info topic
   * @param camera_link Name of the camera link frame
  */
  Camera(
    const std::string & id, as2::Node * node_ptr, const float pub_freq = -1.0f,
    bool add_sensor_measurements_base = true,
    const std::string & info_name = "camera_info",
    const std::string & camera_link = "camera_link");

  /**
   * @brief Destroy the Camera object
  */
  virtual ~Camera();

  /**
   * @brief Update the data of the camera
   *
   * @param img Image message
  */
  void updateData(const sensor_msgs::msg::Image & img);

  /**
   * @brief Update the data of the camera
   *
   * @param img Image message
  */
  void updateData(const cv::Mat & img);

  /**
   * @brief Set camera info parameters
   *
   * @param camera_info Camera info message
   * @param encoding Image encoding
   * @param camera_model Camera model (default is "pinhole")
  */
  void setParameters(
    const sensor_msgs::msg::CameraInfo & camera_info, const std::string & encoding,
    const std::string & camera_model = "pinhole");

  /**
   * @brief Set the Static Transform in TF
   *
   * @param frame_id Frame ID
   * @param parent_frame_id Parent Frame ID
   * @param x X position (m)
   * @param y Y position (m)
   * @param z Z position (m)
   * @param qx Quaternion X
   * @param qy Quaternion Y
   * @param qz Quaternion Z
   * @param qw Quaternion W
  */
  void setStaticTransform(
    const std::string & frame_id, const std::string & parent_frame_id, float x, float y, float z,
    float qx, float qy, float qz, float qw);  // Shadowing the TFStatic function

  /**
   * @brief Set the Static Transform in TF
   *
   * @param frame_id Frame ID
   * @param parent_frame_id Parent Frame ID
   * @param x X position (m)
   * @param y Y position (m)
   * @param z Z position (m)
   * @param roll Roll (rad)
   * @param pitch Pitch (rad)
   * @param yaw Yaw (rad)
  */
  void setStaticTransform(
    const std::string & frame_id, const std::string & parent_frame_id, float x, float y, float z,
    float roll, float pitch, float yaw);  // Shadowing the TFStatic function

  // TODO(perezsaura-david): Implement the following functions
  // void loadParameters(const std::string & file)
  // void publishRectifiedImage(const sensor_msgs::msg::Image & msg)
  // void publishCompressedImage(const sensor_msgs::msg::Image & msg)

private:
  as2::Node * node_ptr_ = nullptr;
  std::string camera_link_;

  std::string camera_base_topic_;
  std::string camera_frame_;

  // Camera info
  std::shared_ptr<SensorData<sensor_msgs::msg::CameraInfo>> camera_info_sensor_;
  bool setup_ = false;
  bool camera_info_available_ = false;
  std::string encoding_;
  std::string camera_model_;
  std::string camera_name_;

  // Camera image
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
  std::shared_ptr<image_transport::ImageTransport> image_transport_ptr_ = nullptr;
  image_transport::Publisher it_publisher_;
  sensor_msgs::msg::Image image_data_;

  /**
   * @brief Get the Node Pointer object
  */
  std::shared_ptr<rclcpp::Node> getSelfPtr();

  /**
   * @brief Setup the camera info
  */
  void setup();

  /**
   * @brief Publish the camera image and camera info
  */
  void publishData();
};  // class CameraSensor

/**
 * @brief Class to handle the ground truth of the platform
 *
*/
class GroundTruth : protected GenericSensor
{
public:
  /**
   * @brief Construct a new GroundTruth object
   *
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
   * @param topic_name_base Base name of the topics. Default is ""
  */
  explicit GroundTruth(
    as2::Node * node_ptr, const float pub_freq = -1,
    const std::string & topic_name_base = "");

  /**
   * @brief Destroy the GroundTruth object
   *
  */
  virtual ~GroundTruth();

  /**
   * @brief Update the data of the ground truth
   *
   * @param pose_msg Pose message
  */
  void updateData(const geometry_msgs::msg::PoseStamped & pose_msg);

  /**
   * @brief Update the data of the ground truth
   *
   * @param twist_msg Twist message
  */
  void updateData(const geometry_msgs::msg::TwistStamped & twist_msg);

  /**
   * @brief Update the data of the ground truth
   *
   * @param pose_msg Pose message
   * @param twist_msg Twist message
  */
  void updateData(
    const geometry_msgs::msg::PoseStamped & pose_msg,
    const geometry_msgs::msg::TwistStamped & twist_msg);

protected:
  std::shared_ptr<SensorData<geometry_msgs::msg::PoseStamped>> pose_sensor_;
  std::shared_ptr<SensorData<geometry_msgs::msg::TwistStamped>> twist_sensor_;

  void publishData() override;
};  // class GroundTruth

/**
 * @brief Class to handle the gimbal sensor
 *
*/
class Gimbal : public TFStatic, protected TFDynamic, protected GenericSensor,
  protected SensorData<geometry_msgs::msg::PoseStamped>
{
public:
  /**
   * @brief Construct a new Gimbal object
   *
   * @param gimbal_id Frame ID of the gimbal
   * @param gimbal_base_id Frame ID of the gimbal base
   * @param node_ptr Pointer to the node
   * @param pub_freq Frequency to publish the data (-1 to publish every time updateData is called)
   * @param add_sensor_measurements_base Add "sensor_measurements" to the topic name
  */
  explicit Gimbal(
    const std::string & gimbal_id,
    const std::string & gimbal_base_id,
    as2::Node * node_ptr,
    const float pub_freq = -1.0f,
    bool add_sensor_measurements_base = true);

  /**
   * @brief Destroy the Gimbal object
   *
  */
  virtual ~Gimbal();

  /**
   * @brief Set the gimbal base transformation respect to the parent frame
   *
   * @param gimbal_base_transform Transform message
   * @param gimbal_parent_frame_id Parent frame ID (default is "base_link")
  */
  void setGimbalBaseTransform(
    const geometry_msgs::msg::Transform & gimbal_base_transform,
    const std::string & gimbal_parent_frame_id = "base_link");

  /**
   * @brief Update the gimbal transformation respect to the base
   *
   * @param pose_msg Pose message
  */
  void updateData(const geometry_msgs::msg::PoseStamped & pose_msg);

  /**
   * @brief Update the gimbal transformation respect to the base
   *
   * @param pose_msg Pose message
  */
  void updateData(const geometry_msgs::msg::QuaternionStamped & orientation_msg);

  /**
   * @brief Get the Gimbal Frame ID
   * @return const std::string& Frame ID of the gimbal
  */
  const std::string & getGimbalFrameId() const;

  /**
   * @brief Get the Gimbal Base Frame ID
   * @return const std::string& Frame ID of the gimbal base
  */
  const std::string & getGimbalBaseFrameId() const;

protected:
  std::string gimbal_frame_id_;
  std::string gimbal_base_frame_id_;
  geometry_msgs::msg::TransformStamped gimbal_transform_;

  /**
   * @brief Publish the data in a topic and in TF
  */
  void publishData();
};  // class Gimbal

using Odometry = Sensor<nav_msgs::msg::Odometry>;
using Imu = Sensor<sensor_msgs::msg::Imu>;
using GPS = Sensor<sensor_msgs::msg::NavSatFix>;
using Lidar = Sensor<sensor_msgs::msg::LaserScan>;
using Battery = Sensor<sensor_msgs::msg::BatteryState>;
using Barometer = Sensor<sensor_msgs::msg::FluidPressure>;
using Compass = Sensor<sensor_msgs::msg::MagneticField>;
using RangeFinder = Sensor<sensor_msgs::msg::Range>;

}  // namespace sensors
}  // namespace as2

#endif  // AS2_CORE__SENSOR_HPP_
