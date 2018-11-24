#include <ros/ros.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"  
#include "std_msgs/Int8.h"
#include<iostream>
using namespace std;
//回调函数
/*void InitialposeCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
	double x,y,z;
    x= msg->pose.pose.position.x;
    cout<<x<<endl;;
}*/

void tagCallback(const std_msgs::Int8::ConstPtr& msg)
{
  int x;
  x=msg->data;
  cout<<x<<endl;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "node_b");	//初始化ROS，节点命名为node_b，节点名必须唯一。
	ros::NodeHandle n;	//节点句柄实例化
	//ros::Subscriber sub_initialpose = n.subscribe("/initialpose", 1000, InitialposeCallback);	//向话题“str_message”订阅，一旦发布节点（node_a）在该话题上发布消息，本节点就会调用chatterCallbck函数。
  	ros::Subscriber sub_int8 = n.subscribe("/tag", 10, tagCallback);

	ros::spin();	//程序进入循环，直到ros::ok()返回false，进程结束。

	return 0;
}