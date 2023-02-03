set(APPNAME digital_input)

add_executable(${PROJECT_NAME}_${APPNAME} ${APPNAME}.cpp)
target_link_libraries(${PROJECT_NAME}_${APPNAME} udero)
