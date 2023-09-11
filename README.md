# ros2_nodes2

	## Подготовка

### Установка ROS2

    sudo apt update && sudo apt install locales
    sudo locale-gen en_US en_US.UTF-8
    sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
    export LANG=en_US.UTF-8

    sudo apt install software-properties-common
    sudo add-apt-repository universe


    sudo apt update && sudo apt install curl -y
    sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg

    echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null


### Создать папку проекта в рабочем пространстве ~/ros2_ws/src/

### Настроил bash, добавив в .bashrc :

Настрол работу с ROS2 в терминале:

    source /opt/ros/humble/setup.sh

Настроил автокомпиляцию с помощью сборщика в bashrc:

    source /usr/share/colcon_argcomplete/hook/colcon-argcomplete.bash
 
Настроил работу с рабочим пространством

    source /home/tr4in33/ros2_ws/install/setup.bash


### Исправил ошибку версий setuptools, заменив 59.6.0 на 58.2.0:

    pip3 install setuptools==58.2.0


*Исправление ошибки не найденных файлов, при сборке ( https://jstar0525.tistory.com/228 )*

     export AMENT_PREFIX_PATH=''

## БАЗА

Помещаем склонированные узлы в ~/ros2_ws/src/ 

### Сборка узлов из под папки ~/ros2_ws/src/

	colcon build	

### Запуск узлов

Запуск узла камеры airplane_ros:

	ros2 run airplane_ros flycam_cpp 

Запуск узла затычки nodes_pool_py:

	ros2 run nodes_pool_py subscriber_node
