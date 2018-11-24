#include "ros/ros.h"
#include <iostream>
#include "geometry_msgs/PoseWithCovarianceStamped.h"  

using namespace std;

//话题回调函数geometry_msgs/PoseWithCovarianceStamped

void amclposeCallback(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg)
{
	double amcl_pos_x,amcl_pos_y,amcl_pos_z;
    double amcl_ang_x,amcl_ang_y,amcl_ang_z,amcl_ang_w;
    amcl_pos_x= msg->pose.pose.position.x;
    amcl_pos_y= msg->pose.pose.position.y;
    amcl_pos_z= msg->pose.pose.position.z;
    amcl_ang_x= msg->pose.pose.orientation.x;
    amcl_ang_y= msg->pose.pose.orientation.y;
    amcl_ang_z= msg->pose.pose.orientation.z;
    amcl_ang_w= msg->pose.pose.orientation.w;
    cout<<"pos_X="<<amcl_pos_x<<endl;
    cout<<"pos_Y="<<amcl_pos_y<<endl;
    cout<<"pos_Z="<<amcl_pos_z<<endl;
    cout<<"ang_X="<<amcl_ang_x<<endl;
    cout<<"ang_Y="<<amcl_ang_y<<endl;
    cout<<"ang_Z="<<amcl_ang_z<<endl;
    cout<<"ang_W="<<amcl_ang_w<<endl;
}



int main(int argc, char **argv)
{
	ros::init(argc, argv, "amcl_sub");	//初始化ROS，节点命名为node_b，节点名必须唯一。
	ros::NodeHandle n;	//节点句柄实例化
	ros::Subscriber sub = n.subscribe("/amcl_pose", 10, amclposeCallback);	

	ros::spin();	//程序进入循环，直到ros::ok()返回false，进程结束。

	return 0;
}
