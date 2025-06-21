#include <chrono>
#include <string>
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/kill.hpp"
#include "turtlesim/srv/spawn.hpp"

using namespace std::chrono_literals;

class TurtlesSpawner : public rclcpp::Node {
  public:
    rclcpp::Client<turtlesim::srv::Kill>::SharedPtr turtlesim_kill_client;
    rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr turtlesim_spawn_client;

    TurtlesSpawner() : Node("turtles_spawner") {
        this->turtlesim_kill_client = this->create_client<turtlesim::srv::Kill>("/kill");
        this->turtlesim_spawn_client = this->create_client<turtlesim::srv::Spawn>("/spawn");
        this->killDefaultTurtle();
        this->spawnTurtlesFromCondition();
    }

    bool rclClientStatusMonitor() {
        if (rclcpp::ok()) {
            RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Service is not available, trying...");
            return 0;
        } 
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "ROS interrupted");
        return 1;
        
    }

    void killDefaultTurtle() {
        while (!this->turtlesim_kill_client->wait_for_service(1s)) 
            if (rclClientStatusMonitor()) return;
        auto request = std::make_shared<turtlesim::srv::Kill::Request>();
        request->name = "turtle1";
        auto result = this->turtlesim_kill_client->async_send_request(request);
        if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result) != rclcpp::FutureReturnCode::SUCCESS)
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Service /kill failed");
    }

    void spawnTurtlesFromCondition() {
        while (!this->turtlesim_spawn_client->wait_for_service(1s)) 
            if (rclClientStatusMonitor()) return;
        auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
        float* xs = new float[4];
        float* ys = new float[4];
        float* thetas = new float[4];
        std::string* names = new std::string[4];
        xs[0] = 1.0; xs[1] = 8.0; xs[2] = 9.25; xs[3] = 9.25;
        ys[0] = 1.0; ys[1] = 5.0; ys[2] = 5.0; ys[3] = 5.0;
        thetas[0] = M_PI / 2; thetas[1] = M_PI / 2; thetas[2] = M_PI; thetas[3] = 0.0;
        names[0] = "turtle1"; names[1] = "turtle2"; names[2] = "turtle3"; names[3] = "turtle4";
        for (int i = 0; i < 4; i++) {
            request->x = xs[i];
            request->y = ys[i];
            request->theta = thetas[i];
            request->name = names[i];
            auto result = this->turtlesim_spawn_client->async_send_request(request);
            if (rclcpp::spin_until_future_complete(this->get_node_base_interface(), result) != rclcpp::FutureReturnCode::SUCCESS)
                RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Service /spawn failed");
        }
        delete[] xs; delete[] ys; delete[] thetas; delete[] names;
    }
};

int main(int argc, char** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin_some(std::make_shared<TurtlesSpawner>());
  rclcpp::shutdown();
  return 0;
}