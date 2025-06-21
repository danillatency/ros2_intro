#include "rclcpp/rclcpp.hpp"
#include "homework1_danil_chegaev/msg/twist_array.hpp"
#include "geometry_msgs/msg/twist.hpp"

class MapperNode : public rclcpp::Node {
public:
    rclcpp::Subscription<homework1_danil_chegaev::msg::TwistArray>::SharedPtr subscriber;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr* publishers;

    MapperNode() : Node("mapper_node") {
        this->subscriber = this->create_subscription<homework1_danil_chegaev::msg::TwistArray>("/turtles_cmd", 10,
                           std::bind(&MapperNode::cmdCallback, this, std::placeholders::_1));
        this->publishers = new rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr[4];
        for (int i = 0; i < 4; i++)
            this->publishers[i] = this->create_publisher<geometry_msgs::msg::Twist>("/turtle" + std::to_string(i + 1) + "/cmd_vel", 10);
    }

    void cmdCallback(const homework1_danil_chegaev::msg::TwistArray::SharedPtr message) {
        for (int i = 0; i < 4; i++)
            this->publishers[i]->publish(message->twists[i]);
    }
};


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MapperNode>());
    rclcpp::shutdown();
    return 0;
}
