include_guard(GLOBAL)
message("middleware_dhara component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/dhara/map.c
    ${CMAKE_CURRENT_LIST_DIR}/dhara/journal.c
    ${CMAKE_CURRENT_LIST_DIR}/dhara/error.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/dhara
)


