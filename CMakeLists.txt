cmake_minimum_required(VERSION 3.8)
project(quantum_chess)

set(CMAKE_CXX_STANDARD 17)

# include directories
include_directories(include 
                  ${EIGEN3_INCLUDE_DIR})

# find dependencies
find_package(Eigen3 REQUIRED)


add_executable(game_exec src/game.cpp)
target_link_libraries(game_exec Eigen3::Eigen)

install(TARGETS
  game_exec
  DESTINATION lib/${PROJECT_NAME})
