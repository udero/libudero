set(_srcs SocketCAN.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../libepos/libcanopen/include)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../PCAN_Basic_API/include)

if (WIN32)
# link_directories(${PCANBASIC_LIBDIR})
set(CAN_LIBRARIES  )
else (WIN32)

endif (WIN32)


find_package(Threads)

add_library(${PROJECT_NAME} SHARED ${_srcs})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${CAN_LIBRARIES})

add_library(${PROJECT_NAME}Static STATIC ${_srcs})
target_link_libraries(${PROJECT_NAME}Static ${CMAKE_THREAD_LIBS_INIT} ${CAN_LIBRARIES})

set_property(TARGET ${PROJECT_NAME}Static APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}Static
    EXPORT ${PROJECT_NAME} DESTINATION ${LIB_INSTALL_DIR})
