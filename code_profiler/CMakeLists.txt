cmake_minimum_required(VERSION 3.5)
project(code_profiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Werror=all)
  add_compile_options(-Wextra -Werror=extra)
endif()

find_package(Threads REQUIRED)

find_package(ament_cmake REQUIRED)
find_package(code_profiler_interfaces REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

# 2. Give the library a unique name (Project name + _lib)
add_library(${PROJECT_NAME} SHARED
  src/loop_timer.cpp
  src/profiler.cpp
  src/profiling/ProfileLog.cpp
  src/profiling/StatsPublisher.cpp
  src/profiling/Timer.cpp
  src/ros/profile_publisher.cpp
  src/scoped_timer.cpp
  src/timer.cpp
)

# Link the typesupport of custom messages
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    code_profiler_interfaces__rosidl_typesupport_cpp
    rclcpp::rclcpp
    std_msgs::std_msgs__rosidl_typesupport_cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

# 3. Executables (Make sure they are all defined before the install step)
add_executable(profiler_test src/main.cpp)
target_link_libraries(profiler_test ${PROJECT_NAME} Threads::Threads)

add_executable(test_loop_timer test/test_loop_timer.cpp)
target_link_libraries(test_loop_timer ${PROJECT_NAME})

add_executable(test_profiler test/test_profiler.cpp)
target_link_libraries(test_profiler ${PROJECT_NAME})

# 4. Installation
install(
  TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  PROGRAMS scripts/liveprofile
  DESTINATION lib/${PROJECT_NAME}
)

install(
  TARGETS
    profiler_test
    test_loop_timer
    test_profiler
  DESTINATION lib/${PROJECT_NAME}
)

# 5. Export
ament_export_targets(export_${PROJECT_NAME} HAS_LIBRARY_TARGET)
ament_export_dependencies(code_profiler_interfaces rclcpp std_msgs)

# 6. Testing and Linting
if(BUILD_TESTING)
  find_package(tue_lint_config REQUIRED)
  find_package(ament_cmake_clang_format REQUIRED)
  find_package(ament_cmake_clang_tidy REQUIRED)
  find_package(ament_cmake_lint_cmake REQUIRED)
  find_package(ament_cmake_xmllint REQUIRED)
  ament_clang_format(
    CONFIG_FILE ${tue_lint_config_DIR}/../config/.clang-format
    --clang-format-version=21
  )
  ament_clang_tidy(
    CONFIG_FILE ${tue_lint_config_DIR}/../config/.clang-tidy
    # --fix-errors
    ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR}/compile_commands.json)
  ament_lint_cmake(MAX_LINE_LENGTH 120 "--filter=
    +convention/filename,
    +linelength,
    +package/consistency,
    +package/stdargs,
    +readability/logic,
    +readability/mixedcase,
    +readability/wonkycase,
    +syntax,+whitespace/eol,
    +whitespace/extra,
    +whitespace/indent,
    +whitespace/mismatch,
    +whitespace/newline,
    +whitespace/tabs"
  )
  ament_xmllint(MAX_LINE_LENGTH 120)
endif()

ament_package()
