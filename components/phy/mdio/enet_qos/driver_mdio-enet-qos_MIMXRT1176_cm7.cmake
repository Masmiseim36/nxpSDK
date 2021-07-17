include_guard(GLOBAL)
message("driver_mdio-enet-qos component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/fsl_enet_qos_mdio.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/.
)


include(driver_mdio-common_MIMXRT1176_cm7)

include(driver_enet_qos_MIMXRT1176_cm7)

