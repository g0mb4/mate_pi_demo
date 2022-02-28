# mate_pi_demo

Demonstration package for ROS2. Runs on a Raspberry Pi 4 with Ubuntu 20.04.

User: **ubuntu**

ROS2 workspace: **/home/ubuntu/ros2_ws**

## Install

This package uses the [WiringPI](https://github.com/WiringPi/WiringPi.git) library to interact with the GPIO pin.

To install the library:
```
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
```

The OneWire driver must be enabled in order to use the DS18B20 sensor:
```
sudo chmod u+w /boot/firmware/usercfg.txt
sudo bash -c 'echo "dtoverlay=w1-gpio" >> /boot/firmware/usercfg.txt'
sudo chmod u-w /boot/firmware/usercfg.txt
```

The node must have read/write access to */dev/gpiomem*, the steps to ensure this, where *ubuntu* is the current user:
```
sudo groupadd gpio              # create group 'gpio'
sudo usermod -a -G gpio ubuntu  # add 'ubuntu' user to 'gpio' group
sudo grep gpio /etc/group       # ensure the 'gpio' group contains 'ubuntu'
sudo chown root.gpio /dev/gpiomem # change the ownership of /dev/gpio so gpio is also an owner
sudo chmod g+rw /dev/gpiomem  # add read/write rights to the owner gropus
```

The **last two** steps must be executed on every startup, the *startup/start.sh* does that.

The ROS2 workspace is assumed to be */home/ubuntu/ros2_ws*.
If the worksapce is different modify the path in *startup/start.sh* and in *startup/mate_pi_demo.service*.

The package uses the [example_interfaces](https://github.com/ros2/example_interfaces) package which is usually installed by default, if it is not installed install it using:
```
cd /home/ubuntu/ros2_ws/src
git clone https://github.com/ros2/example_interfaces.git
```
Finally, to install this package type:
```
cd /home/ubuntu/ros2_ws/src
git clone https://github.com/g0mb4/mate_pi_demo.git
cd ..
colcon build
```

### Install the service:

In order to automatically start the package as soon as the system is ready do:

```
sudo cp /home/ubuntu/ros2_ws/src/mate_pi_demo/startup/mate_pi_demo.service /etc/systemd/system/
sudo systemctl start mate_pi_demo
sudo systemctl enable mate_pi_demo
```

### Uninstall the service:
```
sudo systemctl stop mate_pi_demo
sudo systemctl disable mate_pi_demo
sudo rm /etc/systemd/system/mate_pi_demo.service
```

## pi_gpio_srv

Default pinout of *pi_gpio_srv*:

```
 |...  ...|
 | 35----------- R220 ----+----+
 | 37  38 |               |    |
 | 39---------------------+----+
 +--------+
     RPi                   LED
```

The node creates the *change_pin_state* service that allows the user to turn on/off a GPIO pin.

The pin number is the pin number used by the WiringPi library which can be converted from the physical header using:
```
gpio readall
```

## pi_temp_pub

Default pinout of *pi_temp_pub*:
```
    RPi                      DS18B20
 +--------+               +----------+
 |  1--------------+--------3 VCC    |
 |  3   4 |        |      |          |
 |  5   6 |       R4.7k   |          |
 |  7--------------+--------2 SIGNAL |
 |  9-----------------------1 GND    |
 |...  ...|               +----------+
```

The node reads a DS18B20 temperature sensor and publishes its value to the *temperature* topic.

The deivice id can be obtained using:
```
ls -l /sys/bus/w1/devices/
```
where the prefix *28-* must be omitted.
