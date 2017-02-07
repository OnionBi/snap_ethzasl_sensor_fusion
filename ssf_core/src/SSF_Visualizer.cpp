#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

// global variables
unsigned int path_length_=1000;
visualization_msgs::Marker path;
ros::Publisher pub_path;

void pathCallback(const geometry_msgs::PoseWithCovarianceStampedConstPtr & msg);

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ssf_visualizer");
  ros::NodeHandle n;

  pub_path = n.advertise<visualization_msgs::Marker>("ssf_core/path", 1);

  ros::Subscriber sub_path = n.subscribe("ssf_core/pose",1,&pathCallback);

  path.id=0;
  path.lifetime=ros::Duration(1);
  path.header.frame_id = "/image";
  path.header.stamp = ros::Time::now();
  path.ns = "ssf_core";
  path.action = visualization_msgs::Marker::ADD;
  path.type = visualization_msgs::Marker::LINE_STRIP;
  path.color.r=1.0;
  path.color.g=1.0; // yellow
  path.color.a=1.0;
  path.scale.x=0.001;
  path.pose.orientation.w=1.0;

  while(ros::ok())
  {
    pub_path.publish(path);
    //usleep(1e6);
    ros::spinOnce();
  }

  return 0;
}

void pathCallback(const geometry_msgs::PoseWithCovarianceStampedConstPtr & msg)
{
  geometry_msgs::Point p;
  memcpy(&(p.x),&(msg->pose.pose.position.x),sizeof(double)*3);
  path.points.push_back(p);
  while(path.points.size()>path_length_)
    path.points.erase(path.points.begin());
}