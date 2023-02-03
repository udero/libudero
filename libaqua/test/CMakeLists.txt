cmake_minimum_required(VERSION 2.8)


project(test)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_executable(sockettest sockettest.cpp)

if(WIN32)
  target_link_libraries(sockettest wsock32 ws2_32)
endif()


add_executable(serversockettest serversockettest.cpp)

if(WIN32)
  target_link_libraries(serversockettest wsock32 ws2_32)
endif()