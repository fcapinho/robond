#  Project 4 - Map My World Robot

This is the fourth project for Udacity Robotics Software Engineer nanodegree.

## Database

RTAB-Map database is stored in the `db` directory with [Git Large File Storage (LFS)](https://git-lfs.github.com/).

## Map

Map file in `maps` directory.

## Instructions

To run the project:

```
roslaunch my_robot world.launch 

roslaunch my_robot mapping.launch

roslaunch my_robot teleop.launch
```

To generate the map

```
roscore

rosrun rtabmap_ros rtabmap _database_path:="my_robot/db/rtabmap.db"

rosrun map_server map_saver map:=grid_map

rosservice call /publish_map 1 1 0
```  