#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

/**
 *  DriveBot class creates a service that receives linear and angular velocities
 *  and apply them to the robot by publishing to the differential_drive_controller
 *  plugin command topic
 */
class DriveBot
{
public:
    DriveBot()
    {
        // Create a publisher to the "/cmd_vel" topic
        motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

        // Define a "command_robot" service
        service = n.advertiseService("/ball_chaser/command_robot", &DriveBot::handle_drive_request, this);
    }

private:
    // "command_robot" service callback
    bool handle_drive_request(ball_chaser::DriveToTarget::Request & req, ball_chaser::DriveToTarget::Response & res)
    {
        ROS_INFO("Command Robot received: linear_x:%1.2f / angular_z:%1.2f", (float)req.linear_x, (float)req.angular_z);

        // Build velocity message from service parameters
        geometry_msgs::Twist motor_command;
        motor_command.linear.x = req.linear_x;
        motor_command.angular.z = req.angular_z;

        // Publish velocity message
        motor_command_publisher.publish(motor_command);

        // Return a response message
        res.msg_feedback = "Robot speed set - x: " + std::to_string(motor_command.linear.x) + " , z: " + std::to_string(motor_command.angular.z);

        ROS_INFO_STREAM(res.msg_feedback);

        return true;
    }

    ros::NodeHandle n;
    ros::Publisher motor_command_publisher;
    ros::ServiceServer service;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "drive_bot");

    DriveBot driveBot;

    ros::spin();

    return 0;
}