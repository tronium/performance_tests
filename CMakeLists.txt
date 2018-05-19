cmake_minimum_required(VERSION 2.8.3)
project(performance_tests)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
)

add_message_files(
   FILES
   SuperAwesome.msg
)

add_service_files(
   FILES
   SetRate.srv
)

generate_messages(
   DEPENDENCIES
   std_msgs
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(publisher_cpp src/publisher.cpp)
add_executable(subscriber_cpp src/subscriber.cpp src/benchmark.cpp)

target_link_libraries( publisher_cpp ${catkin_LIBRARIES} )
target_link_libraries( subscriber_cpp ${catkin_LIBRARIES})

add_dependencies(
	publisher_cpp
	#_performance_tests_generate_messages_check_deps_SetRate
	#_performance_tests_generate_messages_check_deps_SuperAwesome
	#performance_tests_generate_messages_py
	performance_tests_generate_messages_cpp
	#performance_tests_generate_messages
)

add_dependencies(
	subscriber_cpp
	#_performance_tests_generate_messages_check_deps_SetRate
	#_performance_tests_generate_messages_check_deps_SuperAwesome
	#performance_tests_generate_messages_py
	performance_tests_generate_messages_cpp
	#performance_tests_generate_messages
)



install(
	PROGRAMS
	scripts/publisher.py
	scripts/subscriber.py
	scripts/bechmark.py
	DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#	TARGETS ${PROJECT_NAME}
#	TARGETS publisher_cpp
#  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(
# RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#	TARGETS ${PROJECT_NAME}
	TARGETS publisher_cpp subscriber_cpp
 	DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
