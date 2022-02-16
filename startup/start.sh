#!/bin/sh

set -xe

source /opt/ros/foxy/setup.bash
source /home/ubuntu/ros2_ws/install/local_setup.bash

sudo chown root.gpio /dev/gpiomem
sudo chmod g+rw /dev/gpiomem

ros2 launch mate_pi_demo start.launch.py
