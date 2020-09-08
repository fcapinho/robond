#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;
int r, g, b;

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
    int pixel[] = {r, g, b};
    int position[] = {0, 0, 0};

    ROS_INFO("Pixel: R=%i G=%i B=%i", pixel[0], pixel[1], pixel[2]);

    ROS_INFO("Image received: %u x %u (%u) - encoding: %s", img.height, img.width, img.step, img.encoding.c_str());
    for (int i = 0; i < img.height * img.step; i+=3) {
        if ( (pixel[0] == img.data[i]) &&
             (pixel[1] == img.data[i+1]) &&
             (pixel[2] == img.data[i+2]) ) {
            int yPos = i / img.step;
            int xPos = (i % img.step) / 3;
            int posIndex = (xPos / (img.width / 3));
            if (posIndex > 2) continue;
            // ROS_INFO("Pixel found: %i x %i (%i)", yPos, xPos, posIndex);

            position[posIndex] += 1;
        }
    }

    int left = position[0];
    int center = position[1];
    int right = position[2];
    ROS_INFO("Pixel position: left=%i / center=%i / right=%i", left, center, right);

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

    std::string node_name = ros::this_node::getName();
    n.param(node_name + "/ball_r", r, 255);
    n.param(node_name + "/ball_g", g, 255);
    n.param(node_name + "/ball_b", b, 255);

    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    ros::spin();

    return 0;
}