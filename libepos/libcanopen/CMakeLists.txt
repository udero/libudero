cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH  TRUE)

# when building, don't use the install RPATH already
# (but later on when installing)
# SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

project(canopen)# VERSION 1.0.0)

set(PROJECT_VERSION 1.0.0 CACHE STRING "libcanopen version")
#set(PROJECT_VERSION_MAJOR 1)
#set(PROJECT_VERSION_MINOR 0)
#set(PROJECT_VERSION_REVISION 0)
#set(PROJECT_VERSION_CANDIDATE 0)
set(PROJECT_DESCRIPTION "library for canopen")
set(PROJECT_VENDOR "ssr")
set(PROJECT_VENDOR_CONTACT "ysuga@ssr.tokyo")
set(PROJECT_AUTHOR "ssr")
set(PROJECT_AUTHOR_SHORT "ssr")


set(LIB_INSTALL_DIR "lib")

# Subdirectories
add_subdirectory(include)
add_subdirectory(src)

