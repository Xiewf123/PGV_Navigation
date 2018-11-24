#include	<ros/ros.h>
#include	<tf/tf.h>
#include <iostream>
//退出用:ctrl+z
int	main(int	argc,	char**	argv){
//初始化
ros::init(argc,	argv,	"coordinate_transformation");
ros::NodeHandle	node;
std::cout<<"第2部分,定义四元数"<<std::endl;
//2.1	由欧拉角计算四元数
tfScalar	yaw,pitch,roll;
yaw=4.712;
pitch=0;
roll=0;
std::cout<<"欧拉角rpy("<<roll<<","<<pitch<<","<<yaw<<")";
tf::Quaternion	q;
q.setRPY(yaw,pitch,roll);
std::cout<<",转化到四元数q:"<<"("<<q[0]<<","<<q[1]<<","<<q[2]<<","<<q[3]<<")"<<std::endl;
return	0;
}