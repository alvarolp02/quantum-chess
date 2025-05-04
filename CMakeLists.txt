cmake_minimum_required(VERSION 3.8)
project(quantum_chess)


set(CMAKE_BUILD_TYPE Debug) # To use gdb

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -funroll-loops -ffast-math")

# include directories
include_directories(include 
                  ${EIGEN3_INCLUDE_DIR})

# find dependencies
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)

find_package(OpenMP REQUIRED)


add_executable(game_exec src/game.cpp)
target_link_libraries(game_exec Eigen3::Eigen
                                sfml-graphics
                                sfml-window
                                sfml-system
                                sfml-audio 
                                sfml-network
                                yaml-cpp
                                OpenMP::OpenMP_CXX)  

install(TARGETS
  game_exec
  DESTINATION lib/${PROJECT_NAME})

