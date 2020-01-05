
PROJECT(test_symmetric)

FILE(GLOB ${PROJECT_NAME}_files
    ../inc/*.h
    *.c
    *.h
)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_files}
)

target_link_libraries(${PROJECT_NAME}
    test_common
)
