# mate_pi_demo

Demonstaration package for ROS2. Runs on a Raspberry Pi 4 with Ubuntu 20.04.

User: **ubuntu**

ROS2 workspace: **/home/ubuntu/ros2_ws**

## Install

Copy the *mate_pi_demo* to the *src* directory of the ROS2 workspace that is assumed to be */home/ubuntu/ros2_ws*.
If the worksapce is different modify the path in *startup/start.sh* and in *startup/mate_pi_demo.service*.

### Install the service:
```
sudo cp startup/mate_pi_demo.service /etc/systemd/system/
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

The node creates the *change_pin_state* service that allows the user to turn on/off a GPIO pin.

This package uses the [WiringPI](https://github.com/WiringPi/WiringPi.git) library to interact with the GPIO pin.

To install the library:
```
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
```

The node must have read/write access to */dev/gpiomem*, the steps to ensure this, where *ubuntu* is the current user.
```
sudo groupadd gpio              # create group 'gpio'
sudo usermod -a -G gpio ubuntu  # add 'ubuntu' user to 'gpio' group
sudo grep gpio /etc/group       # ensure the 'gpio' group contains 'ubuntu'
sudo chown root.gpio /dev/gpiomem # change the ownership of /dev/gpio so gpio is also an owner
sudo chmod g+rw /dev/gpiomem  # add read/write rights to the owner gropus
```

The **last two** steps must be executed on every startup, the *startup/start.sh* does that.

## pi_temp_pub

The node reads a DS18B20 temperature sensor and publishes its value to the *temperature* topic.

The OneWire driver must be enabled:
```
sudo chmod u+w /boot/firmware/usercfg.txt
sudo bash -c 'echo "dtoverlay=w1-gpio" >> /boot/firmware/usercfg.txt'
sudo chmod u-w /boot/firmware/usercfg.txt
```
