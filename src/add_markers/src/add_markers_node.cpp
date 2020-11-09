#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main(int argc, char** argv) {
    ros::init(argc, argv, "add_markers");
    ros::NodeHandle n;
    ros::Duration d(5);
    ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    uint32_t shape = visualization_msgs::Marker::CUBE;

    visualization_msgs::Marker marker;

    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();

    marker.ns = "add_markers"; // Namespace
    marker.id = 0;             // Id

    marker.type = shape;

    marker.action = visualization_msgs::Marker::ADD;

    // Marker pose
    marker.pose.position.x = 3.5;
    marker.pose.position.y = 4;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.2;
    marker.scale.y = 0.2;
    marker.scale.z = 0.2;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
        if (!ros::ok())
        {
            return 0;
        }
        ROS_WARN_ONCE("Please create a subscriber to the marker");
        sleep(1);
    }

    ROS_INFO("Drop marker at pickup zone");
    marker_pub.publish(marker);

    d.sleep();

    ROS_INFO("Pick up marker");
    marker.action = visualization_msgs::Marker::DELETE;
    marker_pub.publish(marker);

    d.sleep();

    ROS_INFO("Drop marker at drop off zone");
    marker.action = visualization_msgs::Marker::ADD;
    marker.pose.position.x = 3.0;
    marker.pose.position.y = 0.0;
    marker_pub.publish(marker);

    d.sleep();

    return 0;
}