cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
#set(CMAKE_REQUIRED_FLAGS -std=c++14)
# add_definitions(-std=gnu++11)

set(CMAKE_SKIP_BUILD_RPATH  TRUE)

# when building, don't use the install RPATH already
# (but later on when installing)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

project(udero) # VERSION 1.0.0)
set(PROJECT_VERSION 1.0.0 CACHE STRING "libudero version")
set(PROJECT_DESCRIPTION "library for udero")
set(PROJECT_VENDOR "ssr")
set(PROJECT_VENDOR_CONTACT "ysuga@ssr.tokyo")
set(PROJECT_AUTHOR "ssr")
set(PROJECT_AUTHOR_SHORT "ssr")


set(LIB_INSTALL_DIR "lib")

# Subdirectories
if(WIN32)
  add_subdirectory(libpcanbasic)
  set(BUILD_CAN TRUE)
elseif(ALLPE) # If MacOSX, do not build CAN.
  message(BUILD_CAN is FALSE)
  set(BUILD_CAN FALSE)
else(WIN32)
  add_subdirectory(libsocketcan)
  set(BUILD_CAN TRUE)
endif(WIN32)

if (BUILD_CAN)
  add_subdirectory(libepos)
endif (BUILD_CAN)

add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(apps)
if(FALSE)
add_subdirectory(test)
add_subdirectory(sample)
add_subdirectory(devices)

endif(FALSE)
#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}_test)
