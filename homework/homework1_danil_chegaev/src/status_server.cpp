#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"
#include "std_msgs/msg/bool.hpp"

class StatusServer : public rclcpp::Node {
public:
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr service;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr* subscribers;
    bool* actualStatuses;

    StatusServer() : Node("status_server") {
        this->service = this->create_service<std_srvs::srv::Empty>("/status",
                        std::bind(&StatusServer::statusRequestCallback, this, std::placeholders::_1, std::placeholders::_2));
        this->subscribers = new rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr[4];
        for (int i = 0; i < 4; i++)
            this->subscribers[i] = this->create_subscription<std_msgs::msg::Bool>("/turtle" + std::to_string(i + 1) + "_done", 10,
                                   [this, i](const std_msgs::msg::Bool::SharedPtr message) {
                                        this->doneDeterminedCallback(message, i);
                                   });
        this->actualStatuses = new bool[4];
    }

    void statusRequestCallback(const std_srvs::srv::Empty::Request::SharedPtr request, 
                               std_srvs::srv::Empty::Response::SharedPtr response) {
        RCLCPP_INFO(this->get_logger(), "Actual statuses of the four turtles:"
                    "\n\tturtle1: %d\n\tturtle2: %d\n\tturtle3: %d\n\tturtle4: %d", 
                    this->actualStatuses[0], this->actualStatuses[1], this->actualStatuses[2], this->actualStatuses[3]);
    }

    void doneDeterminedCallback(const std_msgs::msg::Bool::SharedPtr message, int fromTurtle) {
        this->actualStatuses[fromTurtle] = message->data;
    }
};


int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<StatusServer>());
    rclcpp::shutdown();
    return 0;
}
