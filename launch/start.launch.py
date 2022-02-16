from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    ld = LaunchDescription()

    ld.add_action(Node(
        package="mate_pi_demo",
        executable="pi_temp_pub",
        parameters= [
            {"device_id": "031397943d74"}
        ]
    ))

    ld.add_action(Node(
        package="mate_pi_demo",
        executable="pi_gpio_srv",
        parameters= [
            {"gpio_pin": 24}
        ]
    ))

    return ld
