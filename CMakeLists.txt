cmake_minimum_required(VERSION 3.5)
project(mate_pi_demo)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

add_executable(pi_gpio_srv src/gpio_srv.cpp)
ament_target_dependencies(pi_gpio_srv rclcpp example_interfaces)
target_link_libraries(pi_gpio_srv ${WIRINGPI_LIBRARIES})

add_executable(pi_temp_pub src/temp_pub.cpp)
ament_target_dependencies(pi_temp_pub rclcpp example_interfaces)

install(TARGETS
  pi_gpio_srv
  pi_temp_pub
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
