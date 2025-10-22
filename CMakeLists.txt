cmake_minimum_required(VERSION 3.16)
project(RobotDDS VERSION 1.0.0 LANGUAGES CXX)

# Standard C++11 minim (FastDDS suportă C++11, dar C++14/17 e recomandat)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Mesaje informative
message(STATUS "Building Robot DDS Project")
message(STATUS " - Publisher application")
message(STATUS " - Subscriber application")

# Adaugă path-uri de căutare pentru FastDDS instalat local
# Ajustează path-urile dacă ai instalat în alt loc
list(APPEND CMAKE_PREFIX_PATH
"$ENV{HOME}/Fast-DDS/install"
"$ENV{HOME}/Fast-CDR/install"
"$ENV{HOME}/foonathan_memory_vendor/install"
"/usr/local"
)

# Găsim FastDDS și FastCDR
# Notă: FastDDS 3.x folosește pachetul "fastdds" (nu "fastrtps")
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
# Fișiere generate din IDL (cod automat generat)
# ============================================================================
# Notă: fastddsgen generează fișiere .cxx (nu .cpp)
set(GENERATED_SOURCES
#generated/RobotTelemetry.cxx
generated/RobotTelemetryPubSubTypes.cxx
)

# Dacă vrei Type Object Support (DDS XTypes), decomentează linia de jos:
# list(APPEND GENERATED_SOURCES generated/RobotTelemetryTypeObjectSupport.cxx)

# ============================================================================
# Library cu codul generat (reutilizabil pentru publisher și subscriber)
# ============================================================================
add_library(robot_telemetry_types STATIC
${GENERATED_SOURCES}
)

# Include-urile și linking-ul sunt gestionate automat prin target_link_libraries cu fastdds și fastcdr
# (eliminăm target_include_directories manual, deoarece imported targets le propagează)

target_link_libraries(robot_telemetry_types
fastdds
fastcdr
)

# ============================================================================
# Library cu RobotSimulator (reutilizabil)
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
# Library cu RobotPublisher
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
# Library cu RobotSubscriber
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
# Executabil Publisher
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
# Executabil Subscriber
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
# Informații post-build
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
# Opțional: Install targets
# ============================================================================
install(TARGETS publisher subscriber
RUNTIME DESTINATION bin
)

# ============================================================================
# Opțional: Print summary după build
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