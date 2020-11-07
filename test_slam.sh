#!/bin/sh
xterm  -e  "ROBOT_INITIAL_POSE='-y -4 -x 2' roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$PWD/src/world/test_building.world" &