This package uses 'WiringPI' library to interact with the GPIOs and the DS18B20 sensor.

To install the library:
    git clone https://github.com/WiringPi/WiringPi.git
    cd WiringPi
    ./build

## pi_gpio_srv

The node must have read/write access to '/dev/gpiomem', the steps to ensure this:
    sudo groupadd gpio              # create user group 'gpio'
    sudo usermod -a -G gpio ubuntu  # add 'ubuntu' user to 'gpio'
    sudo grep gpio /etc/group       # ensure the 'gpio' group contains 'ubuntu'
    + sudo chown root.gpio /dev/gpiomem # change the ownership of /dev/gpio so gpio is also an owner
    + sudo chmod g+rw /dev/gpiomem  # add read/write rights to the owner gropus

The steps marked with '+' must be executed on every startup.

## pi_temp_pub

The node reads a DS18B20 temperature sensor and publishes its value to the "temperature" topic.

The OneWire driver must be enabled:
    Edit "/boot/firmware/usercfg.txt"
    Add line: dtoverlay=w1-gpio
