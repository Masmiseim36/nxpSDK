include_guard()
message("middleware_lwip_enet_ethernetif component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ethernetif.c
    ${CMAKE_CURRENT_LIST_DIR}/port/ethernetif_mmac.c
)

if(CONFIG_USE_driver_enet_MIMXRT1064)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/enet_ethernetif_kinetis.c
)
else()
    message(WARNING "please config platform.drivers.enet_MIMXRT1064 or driver.lpc_enet_MIMXRT1064 or driver.enet_qos_MIMXRT1064 first.")
endif()


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/port
)


#OR Logic component
if(CONFIG_USE_driver_enet_MIMXRT1064)
     include(driver_enet_MIMXRT1064)
endif()
if(NOT (CONFIG_USE_driver_enet_MIMXRT1064))
    message(WARNING "Since driver_enet_MIMXRT1064 is not included at first or config in config.cmake file, use driver_enet_MIMXRT1064 by default.")
    include(driver_enet_MIMXRT1064)
endif()

include(middleware_lwip_MIMXRT1064)

include(driver_phy-common_MIMXRT1064)

