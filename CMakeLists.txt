cmake_minimum_required(VERSION 3.16)
project(RobotDDS VERSION 1.0.0 LANGUAGES CXX)

# Minimum C++ standard (FastDDS requires at least C++11, C++14/17 recommended)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Informative messages
message(STATUS "Building Robot DDS Project")
message(STATUS "  - Publisher application")
message(STATUS "  - Subscriber application (coming soon)")

# Find FastDDS dependencies
find_package(fastdds REQUIRED)
find_package(fastcdr REQUIRED)

# Include directories (global, dar mai bine per-target; lasă-l deocamdată)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/generated
)

# --------------------------------------------------------------------------
# Generated sources from IDL (automatic code)
# --------------------------------------------------------------------------
set(GENERATED_SOURCES
    generated/RobotTelemetryPubSubTypes.cxx
)

# --------------------------------------------------------------------------
# Library for generated types (reusable by publisher and subscriber)
# --------------------------------------------------------------------------
add_library(robot_telemetry_types STATIC
    ${GENERATED_SOURCES}
)

# AICI: Adaugă include-urile din FastDDS (calea către QosPolicies.hpp)
target_include_directories(robot_telemetry_types PUBLIC
    ${fastdds_INCLUDE_DIRS}
)

target_link_libraries(robot_telemetry_types
    fastdds  # Schimbat din fastrtps – adaugă automat mai multe include-uri
    fastcdr
)

# --------------------------------------------------------------------------
# RobotSimulator library (reusable)
# --------------------------------------------------------------------------
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

# --------------------------------------------------------------------------
# RobotPublisher library
# --------------------------------------------------------------------------
add_library(robot_publisher STATIC
    src/RobotPublisher.cpp
)

target_include_directories(robot_publisher PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/generated
    ${fastdds_INCLUDE_DIRS}  # AICI: Adaugă include-urile din FastDDS
)

target_link_libraries(robot_publisher
    robot_telemetry_types
    fastdds  # Schimbat din fastrtps
    fastcdr
)

# --------------------------------------------------------------------------
# Publisher executable
# --------------------------------------------------------------------------
add_executable(publisher
    src/publisher_main.cpp
)

target_include_directories(publisher PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/generated
    ${fastdds_INCLUDE_DIRS}  # AICI: Pentru siguranță, propagă la executable
)

target_link_libraries(publisher
    robot_publisher
    robot_simulator
    robot_telemetry_types
    fastdds  # Schimbat din fastrtps
    fastcdr
)

# --------------------------------------------------------------------------
# Post-build information
# --------------------------------------------------------------------------
message(STATUS "")
message(STATUS "Build configuration:")
message(STATUS "  - C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  - Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  - Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "")
message(STATUS "To build:")
message(STATUS "  mkdir -p build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  make -j$(nproc)")
message(STATUS "")
message(STATUS "To run publisher:")
message(STATUS "  ./publisher")
message(STATUS "")

# --------------------------------------------------------------------------
# Optional: install targets
# --------------------------------------------------------------------------
install(TARGETS publisher
    RUNTIME DESTINATION bin
)

# --------------------------------------------------------------------------
# Optional: print summary after build
# --------------------------------------------------------------------------
add_custom_target(build_info ALL
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo "======================================"
    COMMAND ${CMAKE_COMMAND} -E echo "  Build Complete!"
    COMMAND ${CMAKE_COMMAND} -E echo "======================================"
    COMMAND ${CMAKE_COMMAND} -E echo "Executables:"
    COMMAND ${CMAKE_COMMAND} -E echo "  - publisher: ./publisher"
    COMMAND ${CMAKE_COMMAND} -E echo ""
    DEPENDS publisher
)