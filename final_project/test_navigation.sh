#!/bin/sh

# Launch TurtleBot in Gazebo
xterm  -e  "roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$PWD/src/world/test_building.world" &

# Start AMCL
sleep 5
xterm -e "TURTLEBOT_GAZEBO_MAP_FILE=$PWD/src/map/test_map.yaml roslaunch turtlebot_gazebo amcl_demo.launch" &

# Visualize navigation
sleep 5
xterm -e "roslaunch turtlebot_rviz_launchers view_navigation.launch" &