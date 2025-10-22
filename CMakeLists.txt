cmake_minimum_required(VERSION 3.16)
project(RobotDDS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "Building Robot DDS Project")
message(STATUS " - Publisher application")
message(STATUS " - Subscriber application")

list(APPEND CMAKE_PREFIX_PATH
"$ENV{HOME}/Fast-DDS/install"
"$ENV{HOME}/Fast-CDR/install"
"$ENV{HOME}/foonathan_memory_vendor/install"
"/usr/local"
)

find_package(fastdds REQUIRED)
find_package(fastcdr REQUIRED)

message(STATUS "FastDDS found: ${fastdds_FOUND}")
message(STATUS "FastCDR found: ${fastcdr_FOUND}")

# Include directories
include_directories(
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/generated
)

# ============================================================================
# FIles generated from IDL
# ============================================================================
set(GENERATED_SOURCES
#generated/RobotTelemetry.cxx
generated/RobotTelemetryPubSubTypes.cxx
)

# ============================================================================
# Library cu codul generat (reutilizabil pentru publisher și subscriber)
# ============================================================================
add_library(robot_telemetry_types STATIC
${GENERATED_SOURCES}
)


target_link_libraries(robot_telemetry_types
fastdds
fastcdr
)

# ============================================================================
# Library with RobotSimulator
# ============================================================================
add_library(robot_simulator STATIC
src/RobotSimulator.cpp
)

target_include_directories(robot_simulator PUBLIC
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/generated
)

target_link_libraries(robot_simulator
robot_telemetry_types
)

# ============================================================================
# Library with RobotPublisher
# ============================================================================
add_library(robot_publisher STATIC
src/RobotPublisher.cpp
)

target_include_directories(robot_publisher PUBLIC
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/generated
)

target_link_libraries(robot_publisher
robot_telemetry_types
fastdds
fastcdr
)

# ============================================================================
# Library with RobotSubscriber
# ============================================================================
add_library(robot_subscriber STATIC
src/RobotSubscriber.cpp
)

target_include_directories(robot_subscriber PUBLIC
${PROJECT_SOURCE_DIR}/include
${PROJECT_SOURCE_DIR}/generated
)

target_link_libraries(robot_subscriber
robot_telemetry_types
fastdds
fastcdr
)

# ============================================================================
# Publosher exec
# ============================================================================
add_executable(publisher
src/publisher_main.cpp
)

target_link_libraries(publisher
robot_publisher
robot_simulator
robot_telemetry_types
fastdds
fastcdr
)

# ============================================================================
# Subscriber exec
# ============================================================================
add_executable(subscriber
src/subscriber_main.cpp
)

target_link_libraries(subscriber
robot_subscriber
robot_telemetry_types
fastdds
fastcdr
)

# ============================================================================
#  Post-build infos
# ============================================================================
message(STATUS "")
message(STATUS "Build configuration:")
message(STATUS " - C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS " - Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS " - Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")

message(STATUS "To build:")
message(STATUS " mkdir -p build && cd build")
message(STATUS " cmake ..")
message(STATUS " make -j$(nproc)")
message(STATUS "")

message(STATUS "To run publisher:")
message(STATUS " ./publisher")
message(STATUS "")

# ============================================================================
# Optional: Install targets
# ============================================================================
install(TARGETS publisher subscriber
RUNTIME DESTINATION bin
)

# ============================================================================
# Print summary after build
# ============================================================================
add_custom_target(build_info ALL
COMMAND ${CMAKE_COMMAND} -E echo ""
COMMAND ${CMAKE_COMMAND} -E echo "======================================"
COMMAND ${CMAKE_COMMAND} -E echo " Build Complete!"
COMMAND ${CMAKE_COMMAND} -E echo "======================================"
COMMAND ${CMAKE_COMMAND} -E echo "Executables:"
COMMAND ${CMAKE_COMMAND} -E echo " - publisher: ./publisher"
COMMAND ${CMAKE_COMMAND} -E echo " - subscriber: ./subscriber"
COMMAND ${CMAKE_COMMAND} -E echo ""
DEPENDS publisher subscriber
)