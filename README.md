# Final Project - Home Service Robot

This project aims to build a Home Service Robot in ROS.

## Setup

The project was developed and run on the [Robotics Engineering Nano Degree VMware virtual machine](https://s3-us-west-1.amazonaws.com/udacity-robotics/Virtual+Machines/Lubuntu_071917/RoboVM_V2.1.0.zip) from the [Part 1 - Lesson 6](https://classroom.udacity.com/nanodegrees/nd209/parts/0778207d-f34a-4178-8ccf-9e06b5bd2203/modules/5d87733b-d153-475c-819a-3ee67aac986a/lessons/e0c61e8d-7eac-4807-8737-d2bd321ae7a2/concepts/e8451596-f9a4-4ac3-8aca-e40d7d60efcc).
This image runs Ubuntu 16.04.7 LTS ([Xenial Xerus](http://releases.ubuntu.com/16.04/)).

Installed `xterm` to be consistent with the project description of a launch shell script. For all other interaction, `terminator` was used.

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
$ mkdir src/scripts
$ mkdir src/rvizConfig
$ mkdir src/pick_objects
$ mkdir src/add_markers
```

### TurtleBot

### Rviz

### Gazebo

[Gazebo](http://gazebosim.org/) is a 3D simulation environment aimed to robot systems development and testing.