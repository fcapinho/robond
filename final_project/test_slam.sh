#!/bin/sh

# Launch TurtleBot in Gazebo
xterm  -e  "roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$PWD/src/world/test_building.world" &

# Start SLAM
sleep 5
xterm -e "roslaunch turtlebot_gazebo gmapping_demo.launch" &

# Visualize mapping
sleep 5
xterm -e "roslaunch turtlebot_rviz_launchers view_navigation.launch" &

# Start Turtlebot keyboard teleoperation
xterm -e "roslaunch turtlebot_teleop keyboard_teleop.launch" &