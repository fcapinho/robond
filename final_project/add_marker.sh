#!/bin/sh

# Launch TurtleBot in Gazebo
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

sleep 5

# Publish markers
rosservice call /add_markers/drop_object "x: 3.5
y: 4.0"
sleep 5
rosservice call /add_markers/pick_object "x: 3.5 
y: 4.0"
sleep 5
rosservice call /add_markers/drop_object "x: 3.0 
y: 0.0"