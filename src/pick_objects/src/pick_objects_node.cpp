#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "add_markers/AddMarkersObject.h"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

ros::ServiceClient client_drop;
ros::ServiceClient client_pick;

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
    ros::NodeHandle n;

    double pu_x, pu_y, do_x, do_y;
    
    n.getParam("/pick_objects/pick_up_x", pu_x);
    n.getParam("/pick_objects/pick_up_y", pu_y);
    n.getParam("/pick_objects/drop_off_x", do_x);
    n.getParam("/pick_objects/drop_off_y", do_y);

    ROS_INFO_STREAM("Parameters: (" << pu_x << "," << pu_y << ") -> (" << do_x << "," << do_y << ")");

    client_drop = n.serviceClient<add_markers::AddMarkersObject>("/home_service/drop_object");
    client_pick = n.serviceClient<add_markers::AddMarkersObject>("/home_service/pick_object");

    MoveBaseClient moveAC("move_base", true);

    while(!moveAC.waitForServer(ros::Duration(5.0))) {
        ROS_INFO("Waiting for the move_base action service to come up");
    }

    ROS_INFO_STREAM("Drop object");
    add_markers::AddMarkersObject srv;
    srv.request.x = pu_x;
    srv.request.y = pu_y;
    if (!client_drop.call(srv))
        ROS_ERROR("Failed to call service safe_move");


    ROS_INFO("Sending first goal");
    if (moveToTarget(moveAC, pu_x, pu_y, 1.0) == false) {
        return 1;
    }

    ROS_INFO_STREAM("Pick object");
    if (!client_pick.call(srv))
        ROS_ERROR("Failed to call service safe_move");

    ROS_INFO("Wait 5 seconds");
    ros::Duration(5.0).sleep();
    
    ROS_INFO("Sending second goal");
    if (moveToTarget(moveAC, do_x, do_y, 1.0) == false) {
        return 1;
    }

    ROS_INFO_STREAM("Drop object");
    srv.request.x = do_x;
    srv.request.y = do_y;
    if (!client_drop.call(srv))
        ROS_ERROR("Failed to call service safe_move");

    ros::Duration(10.0).sleep();

    return 0;
}