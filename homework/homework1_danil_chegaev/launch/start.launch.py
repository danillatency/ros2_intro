from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    turtlesim_node = Node(
        package="turtlesim",
        executable="turtlesim_node",
        name="turtlesim_node"
    )

    turtles_spawner_node = Node(
        package="homework1_danil_chegaev",
        executable="turtles_spawner",
        name="turtles_spawner_node"
    )

    commander_node = Node(
        package="homework1_danil_chegaev",
        executable="commander_node",
        name="commander_node"
    )

    mapper_node = Node(
        package="homework1_danil_chegaev",
        executable="mapper_node",
        name="mapper_node"
    )

    collecter_node = Node(
        package="homework1_danil_chegaev",
        executable="collecter_node",
        name="collecter_node"
    )

    start_server_node = Node(
        package="homework1_danil_chegaev",
        executable="start_server",
        name="start_server_node"
    )

    status_server_node = Node(
        package="homework1_danil_chegaev",
        executable="status_server",
        name="status_server_node"
    )

    return LaunchDescription([
        turtlesim_node, 
        turtles_spawner_node, 
        commander_node, 
        mapper_node, 
        collecter_node, 
        start_server_node, 
        status_server_node
    ])
