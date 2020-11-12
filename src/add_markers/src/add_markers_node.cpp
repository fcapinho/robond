#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "add_markers/AddMarkersObject.h"

ros::Publisher marker_pub;
visualization_msgs::Marker marker;

bool handle_drop_object_request(add_markers::AddMarkersObject::Request& req,
    add_markers::AddMarkersObject::Response& res)
{
    uint32_t shape = visualization_msgs::Marker::CUBE;
    
    ROS_INFO("Drop: AddMarkersObjectRequest received - x: %1.2f y: %1.2f", (float)req.x, (float)req.y);

    marker.type = shape;
    marker.action = visualization_msgs::Marker::ADD;

    marker.pose.position.x = req.x;
    marker.pose.position.y = req.y;

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
    marker_pub.publish(marker);

    // Return a response message
    res.message = "Success";
    res.success = true;
    ROS_INFO_STREAM(res.message);

    return true;
}

bool handle_pick_object_request(add_markers::AddMarkersObject::Request& req,
    add_markers::AddMarkersObject::Response& res)
{
    ROS_INFO("Pick: AddMarkersObjectRequest received - x: %1.2f y: %1.2f", (float)req.x, (float)req.y);

    marker.action = visualization_msgs::Marker::DELETE;

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
    marker_pub.publish(marker);

    // Return a response message
    res.message = "Success";
    res.success = true;
    ROS_INFO_STREAM(res.message);

    return true;
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "add_markers");
    ros::NodeHandle n;
    marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();
    marker.ns = "add_markers"; // Namespace
    marker.id = 0;             // Id

    // Marker pose
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


    ros::ServiceServer pick_service = n.advertiseService("/home_service/pick_object", handle_pick_object_request);
    ros::ServiceServer drop_service = n.advertiseService("/home_service/drop_object", handle_drop_object_request);

    // Handle ROS communication events
    ros::spin();

    return 0;
}