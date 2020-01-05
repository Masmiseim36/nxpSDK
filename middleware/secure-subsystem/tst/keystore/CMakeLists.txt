
PROJECT(test_keystore)

FILE(GLOB ${PROJECT_NAME}_files
    *.c
    *.h
)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_files}
)

target_link_libraries(${PROJECT_NAME}
    test_common
)

CREATE_BINARY()
