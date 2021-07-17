include_guard(GLOBAL)
message("middleware_lwip_enet_ethernetif component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif.c
)

if(CONFIG_USE_driver_enet_MIMXRT1176_cm7)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_kinetis.c
)
elseif(CONFIG_USE_driver_enet_qos_MIMXRT1176_cm7)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_qos.c
)
else()
    message(WARNING "please config platform.drivers.enet_MIMXRT1176_cm7 or driver.lpc_enet_MIMXRT1176_cm7 or platform.drivers.enet_qos_MIMXRT1176_cm7 first.")
endif()


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port
)


#OR Logic component
if(CONFIG_USE_driver_enet_MIMXRT1176_cm7)
     include(driver_enet_MIMXRT1176_cm7)
endif()
if(CONFIG_USE_driver_enet_qos_MIMXRT1176_cm7)
     include(driver_enet_qos_MIMXRT1176_cm7)
endif()
if(NOT (CONFIG_USE_driver_enet_MIMXRT1176_cm7 OR CONFIG_USE_driver_enet_qos_MIMXRT1176_cm7))
    message(WARNING "Since driver_enet_MIMXRT1176_cm7/driver_enet_qos_MIMXRT1176_cm7 is not included at first or config in config.cmake file, use driver_enet_MIMXRT1176_cm7 by default.")
    include(driver_enet_MIMXRT1176_cm7)
endif()

include(middleware_lwip_MIMXRT1176_cm7)

include(driver_phy-common_MIMXRT1176_cm7)

