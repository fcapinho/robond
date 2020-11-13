#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include "add_markers/AddMarkersObject.h"

class PickObjects {
public:
    PickObjects(double _pu_x, double _pu_y, double _do_x, double _do_y) : 
    // Initialize move action
    moveAc("move_base", true)
    {
        // Initialize pick up and drop off positions
        pu_x = _pu_x;
        pu_y = _pu_y;
        do_x = _do_x;
        do_y = _do_y;
        ROS_INFO_STREAM("Parameters: (" << pu_x << "," << pu_y << ") -> (" << do_x << "," << do_y << ")");

        // Object handling services
        client_drop = n.serviceClient<add_markers::AddMarkersObject>("/add_markers/drop_object");
        client_pick = n.serviceClient<add_markers::AddMarkersObject>("/add_markers/pick_object");
    }

    int navigate()
    {
        while (!moveAc.waitForServer(ros::Duration(5.0)))
        {
            ROS_INFO("Waiting for the move_base action service to come up");
        }

        ROS_INFO_STREAM("Drop object");
        drop_object(pu_x, pu_y);

        ROS_INFO("Sending pick up goal");
        if (moveToTarget(pu_x, pu_y) == false)
        {
            return 1;
        }

        ROS_INFO_STREAM("Pick object");
        pick_object(pu_x, pu_y);

        ROS_INFO("Wait 5 seconds");
        ros::Duration(5.0).sleep();

        ROS_INFO("Sending drop off goal");
        if (moveToTarget(do_x, do_y) == false)
        {
            return 1;
        }

        ROS_INFO_STREAM("Drop object");
        drop_object(do_x, do_y);

        return 0;
    }

private:
    bool moveToTarget(double x, double y)
    {
        move_base_msgs::MoveBaseGoal goal;

        // Initialize move target
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();
        goal.target_pose.pose.position.x = x;
        goal.target_pose.pose.position.y = y;
        goal.target_pose.pose.orientation.w = 1.0;

        ROS_INFO_STREAM("Moving to: " << x << ", " << y << ", " << 1.0);
        
        // Call action and wait for the result
        moveAc.sendGoal(goal);
        moveAc.waitForResult();

        // Check action result
        if (moveAc.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        {
            ROS_INFO("Reached goal!");
            return true;
        }
        else
        {
            ROS_INFO("The base failed to reach the goal for some reason");
            return false;
        }
    }

    void pick_object(double x, double y) {
        call_service(client_pick, x, y);
    }

    void drop_object(double x, double y) {
        call_service(client_drop, x, y);
    }

    void call_service(ros::ServiceClient& service, double x, double y) {
        add_markers::AddMarkersObject srv;
        srv.request.x = x;
        srv.request.y = y;

        if (service.call(srv)) {
            ROS_DEBUG_STREAM(srv.response.message << " (" << (srv.response.success ? "true" : "false") << ")");
            if (!srv.response.success) {
                ROS_ERROR("Object service failed");
            }
        } else {
            ROS_ERROR("Failed to call object service");
        }
    }

    ros::NodeHandle n;
    ros::ServiceClient client_drop;
    ros::ServiceClient client_pick;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> moveAc;

    double pu_x, pu_y; // Pick up position
    double do_x, do_y; // Drop off position
};

int main(int argc, char** argv) {
    ros::init(argc, argv, "pick_objects");
   
    ros::NodeHandle nh;
    double pu_x, pu_y, do_x, do_y;

    // Check parameters
    if (!nh.hasParam("/pick_objects/pick_up_x") ||
        !nh.hasParam("/pick_objects/pick_up_y") ||
        !nh.hasParam("/pick_objects/drop_off_x") ||
        !nh.hasParam("/pick_objects/drop_off_y"))
    {
        ROS_ERROR("Missing parameters");
        return 1;
    }

    // Read parameters
    nh.getParam("/pick_objects/pick_up_x", pu_x);
    nh.getParam("/pick_objects/pick_up_y", pu_y);
    nh.getParam("/pick_objects/drop_off_x", do_x);
    nh.getParam("/pick_objects/drop_off_y", do_y);

    // Initialize node
    PickObjects pickObjects(pu_x, pu_y, do_x, do_y);

    // Navigate to pick and drop object
    int result = pickObjects.navigate();

    // Wait before exiting to enable caller log verification
    ros::Duration(10.0).sleep();

    return result;
}