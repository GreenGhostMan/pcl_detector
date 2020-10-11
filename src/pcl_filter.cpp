#include <ros/ros.h>
#include <pcl/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <iostream>

class cloudHandler
{
protected:
	ros::NodeHandle nh;
	ros::Subscriber pcl_sub;
	ros::Publisher pcl_pub;
	int kmean;
	double threshold;
public:
	cloudHandler() 
	{
		pcl_sub = nh.subscribe("/camera/depth/points",10,&cloudHandler::cloudCB, this);
		pcl_pub = nh.advertise<sensor_msgs::PointCloud2>("pcl_filtered", 1);		
		nh.getParam("kmean",kmean);
		nh.getParam("threshold",threshold);
	}

	void cloudCB(const sensor_msgs::PointCloud2 &input_msg)
	{
		nh.getParam("kmean",kmean);
		nh.getParam("threshold",threshold);

		pcl::PointCloud<pcl::PointXYZ> cloud;
		pcl::PointCloud<pcl::PointXYZ> cloud_filtered;
		sensor_msgs::PointCloud2 output_msg;

		pcl::fromROSMsg(input_msg, cloud);

		pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sf;
		sf.setInputCloud(cloud.makeShared());
		sf.setMeanK(10);
		sf.setStddevMulThresh(0.1);
		sf.filter(cloud_filtered);

		pcl::toROSMsg(cloud_filtered, output_msg);
		pcl_pub.publish(output_msg);
	}

};

int main(int argc, char** argv)
{
	ros::init(argc,argv,"pcl_filter");
	cloudHandler handler;
	ros::spin();

	return 0;
}