set(APPNAME digital_input)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libepos/libcanopen/libaqua/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)


include_directories(${PROJECT_SOURCE_DIR}/libpcanbasic/include)

link_directories(${PCANBASIC_LIBDIRS})


add_executable(${PROJECT_NAME}_${APPNAME} ${APPNAME}.cpp)
target_link_libraries(${PROJECT_NAME}_${APPNAME} udero )
set_property(TARGET ${PROJECT_NAME}_${APPNAME} APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_${APPNAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

