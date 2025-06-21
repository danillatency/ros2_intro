cmake_minimum_required(VERSION 3.8)
project(homework1_danil_chegaev)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(turtlesim REQUIRED)
find_package(rclcpp REQUIRED)
find_package(homework1_danil_chegaev REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/Start.srv"
  "srv/Stop.srv"
  "msg/TwistArray.msg"
  "msg/TurtlesimPoseArray.msg"
  DEPENDENCIES builtin_interfaces geometry_msgs turtlesim
)

add_executable(commander_node src/commander_node.cpp)
ament_target_dependencies(commander_node rclcpp geometry_msgs std_msgs)

add_executable(start_server src/start_server.cpp)
ament_target_dependencies(start_server rclcpp homework1_danil_chegaev std_msgs)

add_executable(turtles_spawner src/turtles_spawner.cpp)
ament_target_dependencies(turtles_spawner rclcpp turtlesim)

add_executable(collecter_node src/collecter_node.cpp)
ament_target_dependencies(collecter_node rclcpp homework1_danil_chegaev geometry_msgs turtlesim)

add_executable(mapper_node src/mapper_node.cpp)
ament_target_dependencies(mapper_node rclcpp homework1_danil_chegaev geometry_msgs)

add_executable(status_server src/status_server.cpp)
ament_target_dependencies(status_server rclcpp std_srvs std_msgs)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

install(TARGETS
  commander_node
  start_server
  turtles_spawner
  collecter_node
  mapper_node
  status_server
  DESTINATION lib/${PROJECT_NAME})

ament_package()
