#include <rclcpp/rclcpp.hpp>
#include <example_interfaces/msg/float32.hpp>

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <string>

class TemperaturePublisherNode : public rclcpp::Node {
public:
    TemperaturePublisherNode()
      : rclcpp::Node("pi_temp_pub")
    {
        declare_parameter<std::string>("device_id", "031397943d74");
        auto device_id = get_parameter("device_id").as_string();

        snprintf(m_path_buffer, sizeof(m_path_buffer), "%s%s%s", PREFIX, device_id.c_str(), POSTFIX);

        m_publisher = create_publisher<example_interfaces::msg::Float32>("temperature", 10);

        m_timer = create_wall_timer(
            std::chrono::seconds(1),
            std::bind(&TemperaturePublisherNode::publish_temprature, this)
        );

        RCLCPP_INFO(get_logger(), "started");
    }

private:
    bool read_temperature(){
        int fd = open(m_path_buffer, O_RDONLY);
        if(fd == -1){
            RCLCPP_ERROR(get_logger(), "Unable to open '%s': %s", m_path_buffer, strerror(errno));
            return false;
        }

        int n = read(fd, m_buffer, sizeof(m_buffer));   
        if(n <= 0){
            RCLCPP_ERROR(get_logger(), "Unable to read '%s': %s", m_path_buffer, strerror(errno));
            close(fd);
            return false;
        }

        char *ptr = strstr(m_buffer, "t=");
        if(!ptr){
            RCLCPP_ERROR(get_logger(), "Invalid data (no `t=`)");
            close(fd);
            return false;
        }
        
        ptr += 2; // skip "t="
        int int_value = atoi(ptr);

        m_value = (float)int_value / (float)1000.0;

        close(fd);
        return true;
    }

    void publish_temprature(){
        if(read_temperature()){
            auto msg = example_interfaces::msg::Float32();
            msg.data = m_value;
            m_publisher->publish(msg);

            RCLCPP_INFO(get_logger(), "t=%f oC", m_value);
        } else {
            RCLCPP_ERROR(get_logger(), "Unable to read the temperature.");
        }
    }

    const char * PREFIX = "/sys/bus/w1/devices/28-";
    const char * POSTFIX = "/w1_slave";

    char m_path_buffer[256] = {0};
    char m_buffer[1024] = {0};

    float m_value;

    rclcpp::Publisher<example_interfaces::msg::Float32>::SharedPtr m_publisher;
    rclcpp::TimerBase::SharedPtr m_timer;
};

int main(int argc, char ** argv){
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TemperaturePublisherNode>());

    rclcpp::shutdown();
    return 0;
}