include_guard(GLOBAL)
message("middleware_issdk_drivers_ads component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/drivers/ads/kinetis/auto_detection_service.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/drivers/ads/kinetis
    ${CMAKE_CURRENT_LIST_DIR}/drivers/ads
)


