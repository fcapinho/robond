#!/bin/sh

# Home service parameters
xterm  -e  "roslaunch home_service.launch" &

# Launch TurtleBot in Gazebo
sleep 5
xterm  -e  "roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$PWD/src/world/test_building.world" &

# Start AMCL
sleep 5
xterm -e "TURTLEBOT_GAZEBO_MAP_FILE=$PWD/src/map/test_map.yaml roslaunch turtlebot_gazebo amcl_demo.launch" &

# Visualize navigation and marker
sleep 5
xterm -e "roslaunch add_markers view_navigation_marker.launch" &

# Add Rviz markers
sleep 5
xterm -e "rosrun add_markers add_markers_node" &

# Navigate to 2 goals
sleep 5
xterm -e "rosrun pick_objects pick_objects_node" &