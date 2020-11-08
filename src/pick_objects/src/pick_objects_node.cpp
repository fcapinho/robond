#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

bool moveToTarget(MoveBaseClient &mac, double x, double y, double w) {
    move_base_msgs::MoveBaseGoal goal;

    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    goal.target_pose.pose.position.x = x;
    goal.target_pose.pose.position.y = y;
    goal.target_pose.pose.orientation.w = w;

    ROS_INFO_STREAM("Moving to: " << x << ", " << y << ", " << w);
    mac.sendGoal(goal);

    mac.waitForResult();

    if (mac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
        ROS_INFO("Reached goal!");
        return true;
    } else {
        ROS_INFO("The base failed to reach the goal for some reason");
        return false;
    }    
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "pick_objects");

    MoveBaseClient moveAC("move_base", true);

    while(!moveAC.waitForServer(ros::Duration(5.0))) {
        ROS_INFO("Waiting for the move_base action service to come up");
    }

    ROS_INFO("Sending first goal");
    if (moveToTarget(moveAC, 3.5, 4.0, 1.0) == false) {
        return 1;
    }

    ROS_INFO("Wait 5 seconds");
    ros::Duration(5.0).sleep();
    
    ROS_INFO("Sending second goal");
    if (moveToTarget(moveAC, 3.0, 0.0, 1.0) == false) {
        return 1;
    }

    return 0;
}