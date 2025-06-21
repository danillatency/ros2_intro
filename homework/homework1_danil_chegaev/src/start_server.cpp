#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "homework1_danil_chegaev/srv/start.hpp"
#include "homework1_danil_chegaev/srv/stop.hpp"


class StartServer : public rclcpp::Node
{
  public:
    rclcpp::Service<homework1_danil_chegaev::srv::Start>::SharedPtr start_service;
    rclcpp::Service<homework1_danil_chegaev::srv::Stop>::SharedPtr stop_service;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher;
      
    StartServer() : Node("start_server") {
      this->start_service = this->create_service<homework1_danil_chegaev::srv::Start>("/start", 
                            std::bind(&StartServer::start_service_callback, this, 
                            std::placeholders::_1, std::placeholders::_2));
      this->stop_service = this->create_service<homework1_danil_chegaev::srv::Stop>("/stop", 
                           std::bind(&StartServer::stop_service_callback, this,
                           std::placeholders::_1, std::placeholders::_2));
      this->publisher = this->create_publisher<std_msgs::msg::Bool>("/start_topic", 10);
    }

    void start_service_callback(const homework1_danil_chegaev::srv::Start::Request::SharedPtr request,
                                homework1_danil_chegaev::srv::Start::Response::SharedPtr response) {
      auto message = std_msgs::msg::Bool();
      message.data = true;
      this->publisher->publish(message);
      response->success = true;
    }

    void stop_service_callback(const std::shared_ptr<homework1_danil_chegaev::srv::Stop::Request> request,
                               std::shared_ptr<homework1_danil_chegaev::srv::Stop::Response> response) {
      auto message = std_msgs::msg::Bool();
      message.data = false;
      this->publisher->publish(message);
      response->success = false;
    }
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<StartServer>());
  rclcpp::shutdown();
  return 0;
}
