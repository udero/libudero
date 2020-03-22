set(_srcs ${CMAKE_CURRENT_SOURCE_DIR}/EPOS.cpp ${headers})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libcanopen/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libcanopen/libaqua/include)

find_package(Threads)

add_library(${PROJECT_NAME} SHARED ${_srcs})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} canopenStatic)


add_library(${PROJECT_NAME}Static STATIC ${_srcs})
target_link_libraries(${PROJECT_NAME}Static  ${CMAKE_THREAD_LIBS_INIT} canopenStatic)

set_property(TARGET ${PROJECT_NAME}Static APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
#set_target_properties(${PROJECT_NAME} PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
#    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME} 
  DESTINATION ${LIB_INSTALL_DIR})

install(TARGETS ${PROJECT_NAME}Static
  EXPORT ${PROJECT_NAME}Static 
  DESTINATION ${LIB_INSTALL_DIR})
