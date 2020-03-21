

set(basic_srcs
 ${CMAKE_SOURCE_DIR}/include/udero/UderoLogger.h 
 ${CMAKE_CURRENT_SOURCE_DIR}/UderoLogger.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/UderoFactory.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/UderoImpl.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/kinematics.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/SettingFileParser.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/mock/UderoMock.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/mock/UderoMockJoint.cpp
 )

set(can_srcs 
 ${CMAKE_CURRENT_SOURCE_DIR}/can/UderoCAN.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/can/UderoCANJoint.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/can)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libepos/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libepos/libcanopen/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libepos/libcanopen/libaqua/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)


find_package(Threads)

if (BUILD_CAN)
  set(_srcs ${headers} ${basic_srcs} ${can_srcs})
  if(WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libpcanbasic/include)
    #set(PCANBASIC_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../libpcanbasic/PCAN_Basic_API/Win32/VC_LIB)
    link_directories(${PCANBASIC_LIBDIRS})
    set(CANLIB epos canopen pcanusbbasic)
  else(WIN32)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libsocketcan/include)
    set(CANLIB epos canopen socketcan)
  endif(WIN32)
else (BUILD_CAN)
  set(_srcs ${headers} ${basic_srcs})
  set(CANLIB)
endif (BUILD_CAN)


add_library(${PROJECT_NAME} SHARED ${_srcs})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CANLIB})

#add_library(${PROJECT_NAME}Static STATIC ${_srcs})
#target_link_libraries(${PROJECT_NAME}Static ${CMAKE_THREAD_LIBS_INIT} ${CANLIB})
#set_property(TARGET ${PROJECT_NAME}Static APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)

if (BUILD_CAN)
  set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS BUILD_CAN)  
#  set_property(TARGET ${PROJECT_NAME}Static APPEND PROPERTY COMPILE_DEFINITIONS BUILD_CAN)  
else (BUILD_CAN)

endif (BUILD_CAN)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


install(TARGETS ${PROJECT_NAME} 
    # ${PROJECT_NAME}Static
    EXPORT ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR})


set(sources ${_srcs} PARENT_SCOPE)
