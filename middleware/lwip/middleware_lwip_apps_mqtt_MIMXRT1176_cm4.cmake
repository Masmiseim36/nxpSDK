include_guard()
message("middleware_lwip_apps_mqtt component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/apps/mqtt/mqtt.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/include/lwip/apps
)


include(middleware_lwip_MIMXRT1176_cm4)

