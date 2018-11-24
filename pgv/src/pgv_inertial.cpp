#include <string>
#include <ros/ros.h> // 包含ROS的头文件
#include <tf/transform_broadcaster.h>
#include "geometry_msgs/PoseWithCovarianceStamped.h"  
#include "std_msgs/Int8.h"
#include <iostream>
#include <boost/asio.hpp> //包含boost库函数
#include <boost/bind.hpp>
#include <math.h>
#include "std_msgs/String.h" //ros定义的String数据类型
#include "nav_msgs/Odometry.h"
#include <ctime>
#include <math.h>

using namespace std;
using namespace boost::asio; //定义一个命名空间，用于后面的读写操作
unsigned char buf[24]; //定义字符串长度，返回的数据是24个字节一组，可用串口调试助手获得


double tag_pos_x,tag_pos_y,tag_pos_z;
double tag_ang_x,tag_ang_y,tag_ang_z,tag_ang_w;
double	roll,pitch,yaw;

typedef struct _POSE
{
  double X;
  double Y;
  double Z;
  double or_x;
  double or_y;
  double or_z;
  double or_w;
} POSE;

//二维码坐标
POSE pose1 = {1.200, 0.000, 0.000, 0.000, 0.000, 0.000, 1.000};
POSE pose2 = {2.400, 0.000, 0.000, 0.000, 0.000, 0.000, 1.000};
POSE pose3 = {3.600, 0.00, 0.000, 0.000, 0.000, 0.000, 1.000};
POSE pose4 = {0.000, 0.000, 0.000,  0.000, 0.000, 0.000, 1.000};
//float a[36];//协方差矩阵

float a[36] = {1e-9, 0, 0, 0, 0, 0,
                         0, 1e-3, 1e-9, 0, 0, 0,
                         0, 0, 1e6, 0, 0, 0,
                         0, 0, 0, 1e6, 0, 0,
                         0, 0, 0, 0, 1e6, 0,
                         0, 0, 0, 0, 0, 1e-9};


std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
    const unsigned char c = input[i];
    output.push_back(lut[c >> 4]);
    output.push_back(lut[c & 15]);
    }
    return output;
}

/*void odomposeCallback(const nav_msgs::Odometry::ConstPtr& msg)
{
    
    for(int i=0;i<36;++i)
    {
        a[i]=msg->pose.covariance[i];
        cout<<a[i]<<" "<<endl;
    }
    ros::Duration(5).sleep();
}*/


int main(int argc, char** argv)
{
    ros::init(argc, argv, "boost"); //初始化节点
    ros::NodeHandle n;
    //ros::Publisher pub_initialpose = n.advertise<geometry_msgs::PoseWithCovarianceStamped>("/initialpose", 10);
    ros::Publisher pub_tag = n.advertise<std_msgs::Int8>("/tag", 1);
    ros::Publisher pub_odom = n.advertise<nav_msgs::Odometry>("/odom_revise", 50);
    //ros::Subscriber sub = n.subscribe("/odom", 10, odomposeCallback);	

    ros::Rate loop_rate(100);

    io_service iosev;
    serial_port sp(iosev, "/dev/ttyUSB1"); //定义传输的串口
    sp.set_option(serial_port::baud_rate(115200));
    sp.set_option(serial_port::flow_control());
    sp.set_option(serial_port::parity(serial_port::parity::even));
    sp.set_option(serial_port::stop_bits());
    sp.set_option(serial_port::character_size(8));


    while (ros::ok())
    {

    unsigned char date[4]={0xe4,0x1b,0xc8,0x37};
    write(sp, buffer(date, 4));//命令写入
    read (sp,buffer(buf));
    string str(&buf[0],&buf[23]); //将数组转化为字符串
    std_msgs::String msg;
    std::stringstream ss;
    ss << str;
    msg.data = string_to_hex(ss.str());
    //ROS_INFO("%s", msg.data.c_str()); //打印接受到的字符串

    double X_pos = (buf[7] * 0x80 + buf[8]) & 0x00ff;
    if(X_pos > 127)
        X_pos = X_pos - 256;
    else
        X_pos = X_pos;
    double Y_pos = (buf[9] * 0x80 + buf[10]) & 0x00ff;
    if(Y_pos > 127)
        Y_pos = Y_pos - 256;
    else
        Y_pos = Y_pos;
    unsigned int Angle = buf[13] * 0x80 + buf[14];

    std_msgs::Int8 Tag;
    Tag.data = buf[17] * 0x80 * 0x4000 + buf[18] * 0x4000 + buf[19] * 0x80 + buf[20];
    //cout<<"X偏差值:"<<X_pos/1000<<endl;;
    //cout<<"Y偏差值:"<<Y_pos/1000<<endl;
    //cout<<X_pos/1000<<endl;
    //cout<<"偏航角："<<yaw<<endl;
    cout<<"角度值:"<<Angle<<endl;
    cout<<"标签值："<<Tag<<endl;
    //cout<<"odom_pos_x:"<<odom_pos_x<<endl;
    pub_tag.publish(Tag);

    //cout<<"弧度制："<<Ang<<endl;
   
    /***位置修正***/
    double X ,Y,Ang;
    switch(Tag.data)
    {
    case 6 :
        X =  X_pos/1000;
        Y = Y_pos/1000;
        Ang = 2* M_PI-Angle* M_PI/180;
        break;
    case 7 :
        X =  X_pos/1000+1.2;
        Y = Y_pos/1000;
        Ang = 2*M_PI-Angle*M_PI/180;
        break;
    case 8 :
        X =  X_pos/1000+2.4;
        Y = Y_pos/1000;
        Ang=2* M_PI-Angle* M_PI/180;
        break;
    case 4 :
        X =  X_pos/1000+3.6;
        Y = Y_pos/1000+0.6;
        Ang=2* M_PI-Angle* M_PI/180;
        break;
    case 3 :
        X =  X_pos/1000+4.8;
        Y = Y_pos/1000+1.2;
        Ang=2* M_PI-Angle* M_PI/180;
        break;
    default :
        break;
    };

    //将欧拉角转换成四元数
    geometry_msgs::Quaternion q;
    q = tf::createQuaternionMsgFromRollPitchYaw(0,0,Ang);//三个参数：roll、pitch、yaw
    //cout<<q.x<<" "<<q.y<<" "<<q.z<<" "<<q.w<<endl;
    nav_msgs::Odometry msg_poseinit;
    msg_poseinit.header.frame_id = "map";
    msg_poseinit.header.stamp = ros::Time::now();
    msg_poseinit.pose.pose.position.x = X;
    msg_poseinit.pose.pose.position.y = Y;
    msg_poseinit.pose.pose.position.z = 0;
    msg_poseinit.pose.pose.orientation.x = q.x;
    msg_poseinit.pose.pose.orientation.y = q.y;
    msg_poseinit.pose.pose.orientation.z = q.z;
    msg_poseinit.pose.pose.orientation.w = q.w;
    for(int i=0;i<36;++i)
    {
        msg_poseinit.pose.covariance[i]=a[i];
    }
    cout<<"X=:"<<X<<endl;
    cout<<"Y=:"<<Y<<endl;
    cout<<q.x<<" "<<q.y<<" "<<q.z<<" "<<q.w<<endl;
    if(Tag.data!=0)
       {pub_odom.publish(msg_poseinit);}
    //ros::Duration(2).sleep();
    ros::spinOnce();
    loop_rate.sleep();
    }
    
    iosev.run();
    return 0;
}