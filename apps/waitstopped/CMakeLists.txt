
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)

include_directories(${PROJECT_SOURCE_DIR}/libepos/include)
include_directories(${PROJECT_SOURCE_DIR}/server)
include_directories(${PROJECT_SOURCE_DIR}/libepos/libcanopen/include)
include_directories(${PROJECT_SOURCE_DIR}/libepos/libcanopen/libaqua/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)


include_directories(${PROJECT_SOURCE_DIR}/libpcanbasic/include)
set(PCANBASIC_LIBDIR ${PROJECT_SOURCE_DIR}/libpcanbasic/PCAN_Basic_API/Win32/VC_LIB)

link_directories(${PCANBASIC_LIBDIR})
message(${PCANBASIC_LIBDIR})

add_executable(${PROJECT_NAME}_waitstopped waitstopped.cpp)
target_link_libraries(${PROJECT_NAME}_waitstopped udero)
set_property(TARGET ${PROJECT_NAME}_waitstopped APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_waitstopped PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

