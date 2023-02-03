set(APPNAME joint_get_state)

add_executable(${PROJECT_NAME}_${APPNAME} ${APPNAME}.cpp)
target_link_libraries(${PROJECT_NAME}_${APPNAME} udero)
