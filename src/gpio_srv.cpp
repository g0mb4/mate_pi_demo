#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/srv/set_bool.hpp>

#include <wiringPi.h>

class GPIOSrvNode : public rclcpp::Node {
public:
    GPIOSrvNode() 
      : rclcpp::Node("pi_gpio_srv")
    {
        declare_parameter<int>("gpio_pin", 24);
        m_pin = get_parameter("gpio_pin").as_int();

        if(wiringPiSetup() != 0){
            RCLCPP_FATAL(get_logger(), "wiringPiSetup() failed.");
            exit(1);
        }

        pinMode(m_pin, OUTPUT);
        turn_off_pin();

        m_server = create_service<example_interfaces::srv::SetBool>(
            "change_pin_state",
            std::bind(
                &GPIOSrvNode::serve_change_pin_state,
                this, 
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(get_logger(), "started");
    }

private:
    void turn_on_pin() {
        digitalWrite(m_pin, HIGH);
        RCLCPP_INFO(get_logger(), "GPIO%d turned ON", m_pin);
    }

    void turn_off_pin(){
        digitalWrite(m_pin, LOW);
        RCLCPP_INFO(get_logger(), "GPIO%d turned OFF", m_pin);
    }

    void serve_change_pin_state(
        const example_interfaces::srv::SetBool::Request::SharedPtr request,
        const example_interfaces::srv::SetBool::Response::SharedPtr response)
    {
        if(request->data){
            turn_on_pin();
        } else {
            turn_off_pin();
        }

        response->success = true;
    }

    int m_pin {-1};
    rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr m_server;
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<GPIOSrvNode>());

    rclcpp::shutdown();
    return 0;
}