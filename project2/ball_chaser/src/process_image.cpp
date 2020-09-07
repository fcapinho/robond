#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_robot(float lin_x, float ang_z)
{
    ball_chaser::DriveToTarget srv;

    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;

    if (!client.call(srv)) {
        ROS_ERROR("Failed to call service command_robot");
    }
}

void process_image_callback(const sensor_msgs::Image img)
{
    int white_pixel[] = {255, 255, 255};
    int position[] = {0, 0, 0};

    ROS_INFO("Image received: %u x %u (%u) - encoding: %s", img.height, img.width, img.step, img.encoding.c_str());
    for (int i = 0; i < img.height * img.step; i+=3) {
        if ( (white_pixel[0] == img.data[i]) &&
             (white_pixel[1] == img.data[i+1]) &&
             (white_pixel[2] == img.data[i+2]) ) {
            int yPos = i / img.step;
            int xPos = (i % img.step) / 3;
            int posIndex = (xPos / (img.width / 3));
            if (posIndex > 2) continue;
            // ROS_INFO("White pixel found: %i x %i (%i)", yPos, xPos, posIndex);

            position[posIndex] += 1;
        }
    }

    int left = position[0];
    int center = position[1];
    int right = position[2];
    ROS_INFO("White pixel position: left=%i / center=%i / right=%i", left, center, right);

    if ( (left > 0) || (center > 0) || (right > 0) ) {
        if ( (left > center) && (left > right) ) {
            // Drive left
            ROS_INFO("Drive left");
            drive_robot(0.0, 0.5);
        } else if (center > right) {
            // Drive center
            ROS_INFO("Drive center");
            drive_robot(0.5, 0.0);
        } else {
            // Drive right
            ROS_INFO("Drive right");
            drive_robot(0.0, -0.5);
        }
    } else {
        // Stop
        ROS_INFO("Stop");
        drive_robot(0.0, 0.0);
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}