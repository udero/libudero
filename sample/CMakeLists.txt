set(joy_srcs joytest.cpp)
set(key_srcs keytest.cpp)
set(ledmatrix_srcs ledmatrix_test.cpp)
set(framework_srcs discrete_key_control_main.cpp 
		   discrete_key_control.h 
		   modes/mode_handler.cpp 
		   modes/cartesian_mode.cpp modes/start_mode.cpp modes/joint_mode.cpp
		   control_parameter_holder.cpp udero_controller.cpp)

set(framework_base_srcs 
  discrete_key_control_main_base.cpp 
  discrete_key_control_base.h 
  modes/mode_handler.cpp 
  modes/cartesian_mode.cpp 
  modes/start_mode_base.cpp
  modes/joint_mode_base.cpp
  control_parameter_holder.cpp udero_controller_base.cpp)

set(board_srcs ${framework_srcs} inputs/keyboard_input.cpp)
set(pad_srcs ${framework_srcs} inputs/keypad_input.cpp)
set(pad_base_srcs ${framework_base_srcs} inputs/keypad_input_base.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../server)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libepos/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../thirdparty)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libepos/libcanopen/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libepos/libcanopen/libaqua/include)


# for JOYSTICK

if (FALSE)

if (WIN32)
find_package(SFML 2 COMPONENTS system window)
list(APPEND SFML_LIBRARIES winmm.lib)
include_directories(${SFML_ROOT}/include)
else (WIN32)
if (APPLE)
#list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/thirdparty/SFML-2.5.0/OSX/cmake/Modules)
#find_package(SFML 2 COMPONENTS window system)
# list(APPEND SFML_LIBRARIES winmm.lib)
set(SFML_ROOT ${PROJECT_SOURCE_DIR}/thirdparty/SFML-2.5.0/OSX)
include_directories(${SFML_ROOT}/include)
link_directories(${SFML_ROOT}/lib)
list(APPEND SFML_LIBRARIES sfml-window sfml-system)
endif (APPLE)
endif (WIN32)

endif (FALSE)


# for CAN
if(WIN32)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libpcanbasic/include)
set(PCANBASIC_LIBDIR ${CMAKE_CURRENT_SOURCE_DIR}/../libpcanbasic/PCAN_Basic_API/Win32/VC_LIB)
link_directories(${PCANBASIC_LIBDIR})
set(CANLIB pcanusbbasic)
else(WIN32)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libsocketcan/include)
set(CANLIB socketcan)
endif(WIN32)

find_package(Threads)

if (FALSE)

if(WIN32)
add_executable(${PROJECT_NAME}_joytest ${joy_srcs})
target_link_libraries(${PROJECT_NAME}_joytest ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME} ${SFML_LIBRARIES})
set_property(TARGET ${PROJECT_NAME}_joytest APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_joytest PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif(WIN32)

endif (FALSE)


if (WIN32)

else (WIN32)
if (LINUX)
add_executable(${PROJECT_NAME}_joycontrol joystick_control.cpp)
target_link_libraries(${PROJECT_NAME}_joycontrol ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME} ${SFML_LIBRARIES})
set_property(TARGET ${PROJECT_NAME}_joycontrol APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_joycontrol PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif (LINUX)
endif (WIN32)

add_executable(${PROJECT_NAME}_keyboard ${board_srcs})
target_link_libraries(${PROJECT_NAME}_keyboard ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME}_keyboard APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_keyboard PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}_keypad ${pad_srcs})
target_link_libraries(${PROJECT_NAME}_keypad ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME}_keypad APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_keypad PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(${PROJECT_NAME}_keypad_base ${pad_base_srcs})
target_link_libraries(${PROJECT_NAME}_keypad_base ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME}_keypad_base APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
set_target_properties(${PROJECT_NAME}_keypad_base PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#add_executable(ledmatrix_test ledmatrix_test.cpp)
#set_property(TARGET ledmatrix_test APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
#target_link_libraries(ledmatrix_test ${CMAKE_THREAD_LIBS_INIT})
#set_target_properties(ledmatrix_test PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
#    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#add_executable(abs_test abs_test.cpp)
#set_property(TARGET abs_test APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
#target_link_libraries(abs_test ${CMAKE_THREAD_LIBS_INIT})
#set_target_properties(abs_test PROPERTIES
#    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
#    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


if(FALSE)
add_executable(joystick_test joystick_test.cpp)
set_property(TARGET joystick_test APPEND PROPERTY COMPILE_DEFINITIONS EXPORT_${PROJECT_NAME}_STATIC)
if (APPLE)
set_property(TARGET joystick_test APPEND PROPERTY COMPILE_DEFINITIONS USE_SFML)
endif (APPLE)
target_link_libraries(joystick_test ${CMAKE_THREAD_LIBS_INIT} ${SFML_LIBRARIES})
set_target_properties(joystick_test PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

endif(FALSE)
