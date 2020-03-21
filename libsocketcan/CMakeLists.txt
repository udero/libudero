cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(socketcan) # basic VERSION 1.0.0)


set(PROJECT_VERSION 1.0.0 CACHE STRING "libsocketcan API version")
#set(PROJECT_VERSION_MAJOR 1)
#set(PROJECT_VERSION_MINOR 0)
#set(PROJECT_VERSION_REVISION 0)
#set(PROJECT_VERSION_CANDIDATE 0)
set(PROJECT_DESCRIPTION "library for sockatecan")
set(PROJECT_VENDOR "ssr")
set(PROJECT_VENDOR_CONTACT "ysuga@ssr.tokyo")
set(PROJECT_AUTHOR "ssr")
set(PROJECT_AUTHOR_SHORT "ssr")

set(LIB_INSTALL_DIR "lib")

# Subdirectories
add_subdirectory(include)
add_subdirectory(src)

#install(
#   EXPORT ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR}
#)
