#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "add_markers/AddMarkersObject.h"

class AddMarkers
{
public:
    AddMarkers()
    {
        // Initialize publisher
        marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

        marker.header.frame_id = "map";

        marker.ns = "add_markers"; // Namespace
        marker.id = 0;             // Id

        // Marker pose
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;

        marker.type = visualization_msgs::Marker::CUBE;

        // Set the scale of the marker -- 1x1x1 here means 1m on a side
        marker.scale.x = 0.2;
        marker.scale.y = 0.2;
        marker.scale.z = 0.2;

        // Set the color
        marker.color.r = 0.0f;
        marker.color.g = 0.0f;
        marker.color.b = 1.0f;
        marker.color.a = 1.0;  // alpha: must be non-zero 

        // Object should last forever
        marker.lifetime = ros::Duration();

        // Initialize services
        pick_service = n.advertiseService("/home_service/pick_object", &AddMarkers::handle_pick_object_request, this);
        drop_service = n.advertiseService("/home_service/drop_object", &AddMarkers::handle_drop_object_request, this);
    }

private:
    bool handle_drop_object_request(add_markers::AddMarkersObject::Request &req,
                                    add_markers::AddMarkersObject::Response &res)
    {
        ROS_INFO("Drop: AddMarkersObjectRequest received - x: %1.2f y: %1.2f", (float)req.x, (float)req.y);
     
        marker.pose.position.x = req.x;
        marker.pose.position.y = req.y;

        marker.action = visualization_msgs::Marker::ADD;

        publish_marker(res);

        return true;
    }

    bool handle_pick_object_request(add_markers::AddMarkersObject::Request &req,
                                    add_markers::AddMarkersObject::Response &res)
    {
        ROS_INFO("Pick: AddMarkersObjectRequest received - x: %1.2f y: %1.2f", (float)req.x, (float)req.y);

        marker.action = visualization_msgs::Marker::DELETE;

        publish_marker(res);

        return true;
    }

    void publish_marker(add_markers::AddMarkersObject::Response &res) {
        marker.header.stamp = ros::Time::now();

        if (marker_pub.getNumSubscribers() < 1) {
                // Failure response message
                res.message = "Failure";
                res.success = false;
        } else {
            // Success response message
            res.message = "Success";
            res.success = true;

            marker_pub.publish(marker);
        }

        ROS_INFO_STREAM(res.message);
    }

    ros::NodeHandle n;
    ros::Publisher marker_pub;
    ros::ServiceServer pick_service;
    ros::ServiceServer drop_service;

    visualization_msgs::Marker marker;
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "add_markers");

    AddMarkers addMarkers;

    // Handle ROS communication events
    ros::spin();

    return 0;
}