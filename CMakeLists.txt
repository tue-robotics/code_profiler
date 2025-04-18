cmake_minimum_required(VERSION 3.5)
project(code_profiler)

add_compile_options(-Wall -Werror=all)
add_compile_options(-Wextra -Werror=extra)

find_package(catkin REQUIRED COMPONENTS message_generation roscpp std_msgs)
find_package(Boost REQUIRED COMPONENTS thread system)

add_message_files(FILES
    Statistics.msg
)

generate_messages(DEPENDENCIES)

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
    CATKIN_DEPENDS message_runtime roscpp std_msgs
    DEPENDS Boost
)

###########
## Build ##
###########

include_directories(
    include
    SYSTEM
    ${Boost_INCLUDE_DIRS}
    ${catkin_INCLUDE_DIRS}
)

# Get all the headers
file(GLOB_RECURSE HEADER_FILES include/*.h)

add_library(${PROJECT_NAME}
    ${HEADER_FILES}

    src/loop_timer.cpp
    src/profiler.cpp
    src/profiling/ProfileLog.cpp
    src/profiling/StatsPublisher.cpp
    src/profiling/Timer.cpp
    src/ros/profile_publisher.cpp
    src/scoped_timer.cpp
    src/timer.cpp
)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${catkin_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_DEFINITIONS "PROFILEAPP")
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS})

add_executable(profiler_test src/main.cpp)
target_link_libraries(profiler_test ${PROJECT_NAME})

add_executable(test_profiler test/test_profiler.cpp)
target_link_libraries(test_profiler ${PROJECT_NAME})

add_executable(test_loop_timer test/test_loop_timer.cpp)
target_link_libraries(test_loop_timer ${PROJECT_NAME})

#############
## Install ##
#############

install(
  DIRECTORY include/
  DESTINATION ${CATKIN_GLOBAL_INCLUDE_DESTINATION}
)

install(
  TARGETS
    ${PROJECT_NAME}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

catkin_install_python(PROGRAMS scripts/liveprofile
                      DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

install(
  TARGETS
    profiler_test
    test_loop_timer
    test_profiler
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

#############
## Testing ##
#############

if (CATKIN_ENABLE_TESTING)
  find_package(catkin_lint_cmake REQUIRED)
  catkin_add_catkin_lint_test("-W2 --ignore HEADER_OUTSIDE_PACKAGE_INCLUDE_PATH")
endif()
