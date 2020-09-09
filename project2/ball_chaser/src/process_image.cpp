#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

/**
 *   ProcessImage class detects a color ball by subscribing to the camera sensor image topic 
 *   and then command the robot towards this ball by calling the command robot service
 */
class ProcessImage
{
public:
    ProcessImage()
    {
        // Reads the ball color from the node parameters. Default value is White.
        std::string node_name = ros::this_node::getName();
        n.param(node_name + "/ball_r", r, 255);
        n.param(node_name + "/ball_g", g, 255);
        n.param(node_name + "/ball_b", b, 255);

        // Define a client to request service from command robot
        client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

        // Subscribe to the camera image topic
        subscriber = n.subscribe("/camera/rgb/image_raw", 10, &ProcessImage::process_image_callback, this);
    }

private:
    /**
     *  Command robot velocities
     *
     *  @param lin_x linear velocity
     *  @param ang_z angular velocity
     */
    void drive_robot(float lin_x, float ang_z)
    {
        // Build command velocity request message
        ball_chaser::DriveToTarget srv;
        srv.request.linear_x = lin_x;
        srv.request.angular_z = ang_z;

        // Call command robot service
        if (!client.call(srv))
        {
            ROS_ERROR("Failed to call service command_robot");
        }
    }

    // Camera image topic callback
    void process_image_callback(const sensor_msgs::Image img)
    {
        // Pixel color
        int pixel[] = {r, g, b};
        // Number of pixels on each section of the image: left, center and right
        int position[] = {0, 0, 0};

        ROS_DEBUG("Pixel: R=%i G=%i B=%i", pixel[0], pixel[1], pixel[2]);
        ROS_DEBUG("Image received: %u x %u (%u) - encoding: %s", img.height, img.width, img.step, img.encoding.c_str());

        const int PIXEL_REPRESENTATION_SIZE = 3; // 3 bytes for each pixel ("rgb8")
        const int NUMBER_OF_IMAGE_SECTIONS = 3;  // left, center and right

        // Count the matching color pixels on each section of the image
        for (int i = 0; i < img.height * img.step; i += PIXEL_REPRESENTATION_SIZE)
        {
            if ((pixel[0] == img.data[i]) &&
                (pixel[1] == img.data[i + 1]) &&
                (pixel[2] == img.data[i + 2]))
            {
                // Found a matching pixel

                // Get X and Y position.
                //  - 0x0 is the top left corner (+X is right / +Y is down)
                //  - step is the size of each row
                int yPos = i / img.step;
                int xPos = (i % img.step) / PIXEL_REPRESENTATION_SIZE;

                // Identify the section where the pixel is.
                // Section is defined only by pixel X position.
                int posIndex = (xPos / (img.width / NUMBER_OF_IMAGE_SECTIONS));

                // Make sure each section is of the same size: discard pixels at the far right of the image
                if (posIndex >= NUMBER_OF_IMAGE_SECTIONS)
                    continue;

                position[posIndex] += 1;
            }
        }

        chase_ball(position[0], position[1], position[2]);
    }

    /**
     *  Drive the robot towards the ball
     *
     *  @param left, center, right the number of matching ball color pixels on each image section
     */
    void chase_ball(int left, int center, int right) {
        ROS_DEBUG("Pixels at section: left=%i / center=%i / right=%i", left, center, right);

        // Check if a at least one pixel matched the ball color
        if ((left > 0) || (center > 0) || (right > 0))
        {
            if ((left > center) && (left > right))
            {
                drive_left();
            }
            else if (center > right)
            {
                drive_center();
            }
            else
            {
                drive_right();
            }
        }
        else
        {
            stop();
        }
    }

    void drive_left()
    {
        ROS_DEBUG("Drive left");
        drive_robot(0.0, 0.5);
    }

    void drive_center()
    {
        ROS_DEBUG("Drive center");
        drive_robot(0.5, 0.0);
    }

    void drive_right()
    {
        ROS_DEBUG("Drive right");
        drive_robot(0.0, -0.5);
    }

    void stop()
    {
        ROS_DEBUG("Stop");
        drive_robot(0.0, 0.0);
    }

    ros::NodeHandle n;
    ros::ServiceClient client;
    ros::Subscriber subscriber;

    // Ball color parameters
    int r, g, b;
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "process_image");

    ProcessImage processImage;

    ros::spin();

    return 0;
}