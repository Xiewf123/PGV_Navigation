#include <ros/ros.h>  
#include <move_base_msgs/MoveBaseAction.h>  
#include <actionlib/client/simple_action_client.h>  
#include "geometry_msgs/PoseWithCovarianceStamped.h"     
#include "std_msgs/String.h"

using namespace std;

//0.042, 0.019, 0.000, 0.000, 0.000, 0.000, 1.000
void setHome( ros::Publisher pub_initialpose)
{
    geometry_msgs::PoseWithCovarianceStamped msg_poseinit;
    msg_poseinit.header.frame_id = "map";
    msg_poseinit.header.stamp = ros::Time::now(); 
    msg_poseinit.pose.pose.position.x = 0.0;
    msg_poseinit.pose.pose.position.y = 0.0;
    msg_poseinit.pose.pose.position.z = 0;
    msg_poseinit.pose.pose.orientation.x = 0.0;
    msg_poseinit.pose.pose.orientation.y = 0.0;
    msg_poseinit.pose.pose.orientation.z = 0.0;
    msg_poseinit.pose.pose.orientation.w = 1.0;
    pub_initialpose.publish(msg_poseinit);
    ros::Duration(1.0).sleep();
    pub_initialpose.publish(msg_poseinit);
    ros::Duration(1.0).sleep();
    pub_initialpose.publish(msg_poseinit);
    ros::Duration(1.0).sleep();
}

 /*x: 1.0127655236
      y: 1.99814241133
      z: 0.0
    orientation: 
      x: 0.0
      y: 0.0
      z: 8.56034501716e-05
      w: 0.999999996336*/


int main(int argc, char** argv)
{  
  ros::init(argc, argv, "base_pose_control");  
  ros::NodeHandle nh;
  ros::Publisher pub_initialpose = nh.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10);
  //ros::Rate rate_loop(10);

  setHome(pub_initialpose);
  
 // setGoal(pose1);

      ros::spinOnce();
     // rate_loop.sleep();
  return 0;  
}  
