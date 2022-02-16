# mate_pi_demo

Demonstaration package for ROS2. Runs on a Raspberry Pi 4 with Ubuntu 20.04.

## pi_gpio_srv

The node creates the *change_pin_state* service that allows the user to turn on/off a GPIO pin.

This package uses the [WiringPI](https://github.com/WiringPi/WiringPi.git) library to interact with the GPIO pin.

To install the library:
```
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
```

The node must have read/write access to */dev/gpiomem*, the steps to ensure this:
```
sudo groupadd gpio              # create group 'gpio'
sudo usermod -a -G gpio ubuntu  # add 'ubuntu' user to 'gpio' group
sudo grep gpio /etc/group       # ensure the 'gpio' group contains 'ubuntu'
sudo chown root.gpio /dev/gpiomem # change the ownership of /dev/gpio so gpio is also an owner
sudo chmod g+rw /dev/gpiomem  # add read/write rights to the owner gropus
```

The **last two** steps must be executed on every startup.

## pi_temp_pub

The node reads a DS18B20 temperature sensor and publishes its value to the *temperature* topic.

The OneWire driver must be enabled:
```
sudo chmod u+w /boot/firmware/usercfg.txt
sudo bash -c 'echo "dtoverlay=w1-gpio" >> /boot/firmware/usercfg.txt'
sudo chmod u-w /boot/firmware/usercfg.txt
```
