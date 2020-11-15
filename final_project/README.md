# Final Project - Home Service Robot

This project aims to build a Home Service Robot in ROS. 


## Home Service Robot

The simulated robot is capable of navigating to pick up and deliver virtual objects.

### Environment and dependencies

Robot is simulated on [Gazebo](#gazebo) and managed by the [ROS](#ros) framework, including [Rviz](#rviz) for visualization. [Turtlebot](#turtlebot) model represents the robot.

In order to complete this tasks, the following ROS packages where used:
- [gmapping](#gmapping)
- [turtlebot_teleop](#turtlebot_teleop)
- [turtlebot_rviz_launchers](#turtlebot_rviz_launchers)
- [turtlebot_gazebo](#turtlebot_gazebo)

Please refer to [Setup](#setup) section for a detailed project environment configuration description.

### New Nodes

Two new nodes were created:
- `pick_objects`
- `add_markers`

and connected to other ROS components:

![home service robot nodes](images/home_service.png)

`pick_objects` reads parameters from `rosparam`, calls `move_base` action and `add_markers` services.

`add_markers` publishes messages to `rviz`.

#### Pick Objects Node

Responsible for robot navigation and object handling.

`pick_objects` tasks:
1. Drop off object at pick up position 
   - call `/add_markers/drop_object` service
2. Drive robot to pick up position
   - call `move_base` action
3. Pick up object when position is reached
   - call `/add_markers/pick_object` service
4. Drive robot to drop off position
   - call `move_base` action
5. Drop off object when position is reached
   - call `/add_markers/drop_object` service

The following ROS Parameters configure the task. All coordinates are from the `map` reference.
| Parameter | Type | Description |
|-----------|------|-------------|
| /pick_objects/pick_up_x | float | Pick up X position |
| /pick_objects/pick_up_y | float | Pick up Y position |
| /pick_objects/drop_off_x | float | Drop off X position |
| /pick_objects/drop_off_y | float | Drop off Y position |

#### Add Markers Node

Simulates object pick up and drop off by implementing two services:

| Service | Description |
|---------|-------------|
| /add_markers/pick_object | Pick up object from position |  
| /add_markers/drop_object | Drop off object at position |

defined by `AddMarkersObject.srv`:
```
float32 x
float32 y
---
bool success
string message
```

### Example run

Robot navigate to pick up position:
![pick up navigation](images/home_service_start.png)

Robot pick up the object:
![robot at pick up position](images/home_service_pick.png)

Robot navigate to drop off position:
![drop off navigation](images/home_service_drop.png)

Robot drop off the object:
![final position](images/home_service_final.png)

### Implementation

#### Pick Object Node

- Create new `pick_objects` package:
```shell
$ catkin_create_pkg pick_objects move_base_msgs actionlib roscpp
```

- Add a new `pick_objects_node.cpp` file to implement navigation goal node.

This node sends goals to ROS Navigation Stack by interacting with the [`move_base` package](https://wiki.ros.org/move_base) that implements an [Action](https://wiki.ros.org/actionlib) that given a goal will try to reach it. It also interacts with the add markers node through its servicces.

- Change `CMakeLists.txt` to include the new file.
```cmake
find_package(catkin REQUIRED COMPONENTS actionlib move_base_msgs add_markers roscpp)
include_directories(${catkin_INCLUDE_DIRS})
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} add_markers_generate_messages_cpp)
add_executable(${PROJECT_NAME}_node src/pick_objects_node.cpp)
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})
```

#### Add Markers Node

- Create new `add_markers` package:
```shell
$ catkin_create_pkg add_markers roscpp visualization_msgs
```

- Add a new `add_markers_node.cpp` file to implement virtual object node.

This node handles virtual objects on Rviz by publishing "visualization_marker" messages and adding two new services: "/add_markers/pick_object" and "/add_markers/drop_object" .

- Change `CMakeLists.txt` to include the new files.
```cmake
find_package(catkin REQUIRED COMPONENTS roscpp message_generation visualization_msgs)
add_compile_options(-std=c++11)
add_service_files(FILES AddMarkersObject.srv)
generate_messages(DEPENDENCIES visualization_msgs)
add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} add_markers_generate_messages_cpp)
include_directories(${catkin_INCLUDE_DIRS})
add_executable(${PROJECT_NAME}_node src/add_markers_node.cpp)
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})
``` 

- Change `package.xml` to include message generation build and execution dependencies: 
```xml
  <build_depend>message_generation</build_depend>
  <exec_depend>message_runtime</exec_depend>
```

- Add Rviz Marker

This node requires a Rviz [Marker](https://wiki.ros.org/rviz/DisplayTypes/Marker) display.
Added this display to the navigation configuration and saved this new configuration as `view_navigation_marker.launch`.

![add marker](images/marker.png)


##### Add Markers Script

The Home Service implementation of the `add_markers` node provides two services. The startup marker script was updated to match this implementation. 

To check the original script and implementation, as described on [Modeling Virtual Objects](https://classroom.udacity.com/nanodegrees/nd209/parts/75c8f42b-c844-4f61-b3c6-521956c5cf70/modules/aa8320a9-4fec-4d3b-a3e4-fcd532ba60ff/lessons/1ccf2893-a07b-41c5-b2ed-7cdc48bd26fc/concepts/95f86ccf-73c0-486d-b2d6-8945e6fde3e1), please refer to the `FINAL_PROJECT_ADD_MARKER` git tag.

## Mapping

Start Gazebo with a TurtleBot in the basic building world with `turtlebot_world.launch`.

Run Simultaneous Localization and Mapping (SLAM) with `gmapping_demo.launch` and visualize it with `view_navigation.launch`.

Operate the TurtleBot with the keyboard with `keyboard_teleop.launch`.

```shell
$ source devel/setup.bash
$ ./test_slam.sh
```

![slam test](images/slam_test.png)

### Build a Map

Navigate with the keyboard until a sufficient detailed map is build:
![initial map](images/initial_map.png)

Save the map:
```shell
$ rosrun map_server map_saver -f src/map/test_map
```
 
## Localization and Navigation

Start Gazebo with a TurtleBot in the basic building world with `turtlebot_world.launch`.

Run Adaptive Monte Carlo Localization (AMCL) with `amcl_demo.launch`, referring to the map generated on the [previous](#build-a-map) section.

Visualize and control, through 2D goals, navigation with `view_navigation.launch`.

```shell
$ source devel/setup.bash
$ ./test_navigation.sh
```

![navigation test](images/navigation_test.png)

## Setup

### Development Environment

The project was developed and run on the [Robotics Engineering Nano Degree VMware virtual machine](https://s3-us-west-1.amazonaws.com/udacity-robotics/Virtual+Machines/Lubuntu_071917/RoboVM_V2.1.0.zip) from the [Part 1 - Lesson 6](https://classroom.udacity.com/nanodegrees/nd209/parts/0778207d-f34a-4178-8ccf-9e06b5bd2203/modules/5d87733b-d153-475c-819a-3ee67aac986a/lessons/e0c61e8d-7eac-4807-8737-d2bd321ae7a2/concepts/e8451596-f9a4-4ac3-8aca-e40d7d60efcc).
This image runs Ubuntu 16.04.7 LTS ([Xenial Xerus](http://releases.ubuntu.com/16.04/)).

### Gazebo

[Gazebo](http://gazebosim.org/) is a 3D simulation environment aimed to robot systems development and testing.

Setup and tests are run on the basic shape building from [Part 2 - Lesson 1](https://classroom.udacity.com/nanodegrees/nd209/parts/852e258d-b6c9-4823-b0af-0a7f77379583/modules/8a9ec5d0-dbd1-4f9b-80c5-c01a46aee151/lessons/9b7f4396-2279-4c4b-a785-4142bd00ad8c/concepts/00f1b2c7-0fe6-4bad-9bf6-79efb1a9070d).

![basic building dimensions](images/basic_building.png)
![basic building in Gazebo](images/basic_building.jpg)

### TurtleBot

[TurtleBot](https://www.turtlebot.com/) is a low-cost, personal robot kit with open-source software.

A TurtleBot 2 is used as the simulated robot.

### ROS

The Robot Operating System ([ROS](https://www.ros.org/about-ros/)) is a framework to develop robotic solutions.

This projects runs on ROS version [Kinect Kame](https://wiki.ros.org/kinetic).

#### Development setup

ROS development reside in a [catkin workspace](https://wiki.ros.org/catkin/workspaces).

##### Initialize workspace:
```shell
$ mkdir src && cd src
$ catkin_init_workspace
$ cd ..
$ catkin_make
```

##### Install required packages:
```shell
$ cd src
```

###### [gmapping](https://wiki.ros.org/gmapping)
Provides Simultaneous Localization and Mapping (SLAM) capabilities to a robot equipped with laser range finder and camera sensors.

```shell
$ git clone -b hydro-devel https://github.com/ros-perception/slam_gmapping
```

###### [turtlebot_teleop](https://wiki.ros.org/turtlebot_teleop)
Provides robot keyboard teleoperation

```shell
$ git clone -b kinetic https://github.com/turtlebot/turtlebot
```

###### [turtlebot_rviz_launchers](https://wiki.ros.org/turtlebot_rviz_launchers)
Rviz preconfigured visualization loader for TurtleBot.

```shell
$ git clone -b indigo https://github.com/turtlebot/turtlebot_interactions
```

###### [turtlebot_gazebo](https://wiki.ros.org/turtlebot_gazebo)
Gazebo launcher for TurtleBot.

```shell
$ git clone -b indigo https://github.com/turtlebot/turtlebot_simulator
```

##### Configure packages

```shell
$ cd ..
$ catkin_make
```

##### Create directories

```shell
$ mkdir src/map
$ mkdir src/world
$ mkdir src/pick_objects
$ mkdir src/add_markers
```

### Rviz

[Rviz](https://wiki.ros.org/rviz) is a 3D visualization tool for ROS.