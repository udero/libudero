
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/libepos/libcanopen/libaqua/include)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

add_executable(${PROJECT_NAME}_abs_check abs_check.cpp)
# target_link_libraries(${PROJECT_NAME}_abs_check )
set_property(TARGET ${PROJECT_NAME}_abs_check APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_abs_check PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

