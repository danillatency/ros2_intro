#include <string>
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "homework1_danil_chegaev/msg/twist_array.hpp"
#include "homework1_danil_chegaev/msg/turtlesim_pose_array.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/msg/pose.hpp"

using namespace std::chrono_literals;

class CollecterNode : public rclcpp::Node {
public:
    rclcpp::Publisher<homework1_danil_chegaev::msg::TwistArray>::SharedPtr cmdPublisher;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr* cmdSubscribers;
    homework1_danil_chegaev::msg::TwistArray cmdMessage;
    double*** actualTwistArray;
    rclcpp::Publisher<homework1_danil_chegaev::msg::TurtlesimPoseArray>::SharedPtr posePublisher;
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr* poseSubscribers;
    homework1_danil_chegaev::msg::TurtlesimPoseArray poseMessage;
    double** actualTurtlesimPoseArray;

    CollecterNode() : Node("collecter_node") {
        this->cmdSubscribers = new rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr[4];
        std::string cmdForTopic;
        this->poseSubscribers = new rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr[4];
        std::string poseForTopic;
        for (int i = 0; i < 4; i++) {
            cmdForTopic = "/turtle" + std::to_string(i + 1) + "_cmd";
            this->cmdSubscribers[i] = this->create_subscription<geometry_msgs::msg::Twist>(cmdForTopic, 
                                  10, [this, i](const geometry_msgs::msg::Twist::SharedPtr message) {
                this->determinedCmdCallback(message, i);
            });
            poseForTopic = "/turtle" + std::to_string(i + 1) + "/pose";
            this->poseSubscribers[i] = this->create_subscription<turtlesim::msg::Pose>(poseForTopic, 
                                  10, [this, i](const turtlesim::msg::Pose::SharedPtr message) {
                this->determinedPoseCallback(message, i);
            });
        }
        this->cmdPublisher = this->create_publisher<homework1_danil_chegaev::msg::TwistArray>("/turtles_cmd", 10);
        this->posePublisher = this->create_publisher<homework1_danil_chegaev::msg::TurtlesimPoseArray>("/turtles_positions", 10);
        this->actualTwistArray = new double**[4];
        this->actualTurtlesimPoseArray = new double*[4];
        for (int i = 0; i < 4; i++) {
            this->actualTwistArray[i] = new double*[2];
            this->actualTurtlesimPoseArray[i] = new double[4];
        }
        for (int i = 0; i < 8; i++)
            this->actualTwistArray[i / 2][i % 2] = new double[3];
    }

    void determinedCmdCallback(const geometry_msgs::msg::Twist::SharedPtr message, const int fromTurtle) {
        this->actualTwistArray[fromTurtle][0][0] = message->linear.x;
        this->actualTwistArray[fromTurtle][0][1] = message->linear.y;
        this->actualTwistArray[fromTurtle][0][2] = message->linear.z;
        this->actualTwistArray[fromTurtle][1][0] = message->angular.x;
        this->actualTwistArray[fromTurtle][1][1] = message->angular.y;
        this->actualTwistArray[fromTurtle][1][2] = message->angular.z;
        this->publishCmd();
    }

    void determinedPoseCallback(const turtlesim::msg::Pose::SharedPtr message, const int fromTurtle) {
        this->actualTurtlesimPoseArray[fromTurtle][0] = message->x;
        this->actualTurtlesimPoseArray[fromTurtle][1] = message->y;
        this->actualTurtlesimPoseArray[fromTurtle][2] = message->linear_velocity;
        this->actualTurtlesimPoseArray[fromTurtle][3] = message->angular_velocity;
        this->publishPose();
    }

    void publishCmd() {
        for (int turtleNumber = 0; turtleNumber < 4; turtleNumber++)
            for (int velocityNumber = 0; velocityNumber < 2; velocityNumber++)
                for (int axleNumber = 0; axleNumber < 3; axleNumber++) {
                    double enumeratedValue = this->actualTwistArray[turtleNumber][velocityNumber][axleNumber];
                    if (!velocityNumber) {
                        if (!axleNumber)
                            this->cmdMessage.twists[turtleNumber].linear.x = enumeratedValue;
                        else if (axleNumber == 1)
                            this->cmdMessage.twists[turtleNumber].linear.y = enumeratedValue;
                        else 
                            this->cmdMessage.twists[turtleNumber].linear.z = enumeratedValue;
                    } else {
                        if (!axleNumber)
                            this->cmdMessage.twists[turtleNumber].angular.x = enumeratedValue;
                        else if (axleNumber == 1)
                            this->cmdMessage.twists[turtleNumber].angular.y = enumeratedValue;
                        else 
                            this->cmdMessage.twists[turtleNumber].angular.z = enumeratedValue;
                    }
                }
        this->cmdPublisher->publish(this->cmdMessage);
    }

    void publishPose() {
        for (int turtleNumber = 0; turtleNumber < 4; turtleNumber++)
            for (int parameterNumber = 0; parameterNumber < 4; parameterNumber++) {
                double enumeratedValue = this->actualTurtlesimPoseArray[turtleNumber][parameterNumber];
                switch (parameterNumber) {
                    case 0:
                        this->poseMessage.poses[turtleNumber].x = enumeratedValue; break;
                    case 1:
                        this->poseMessage.poses[turtleNumber].y = enumeratedValue; break;
                    case 2:
                        this->poseMessage.poses[turtleNumber].linear_velocity = enumeratedValue; break;
                    case 3:
                        this->poseMessage.poses[turtleNumber].angular_velocity = enumeratedValue; break;
                }    
            }
        this->posePublisher->publish(this->poseMessage);
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CollecterNode>());
    rclcpp::shutdown();
    return 0;
}
