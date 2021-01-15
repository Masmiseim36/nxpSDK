if(NOT CMSIS_DRIVER_INCLUDE_ETHERNET_MAC_MIMXRT1052_INCLUDED)
    
    set(CMSIS_DRIVER_INCLUDE_ETHERNET_MAC_MIMXRT1052_INCLUDED true CACHE BOOL "CMSIS_Driver_Include_Ethernet_MAC component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Driver_Include_Ethernet_MIMXRT1052)

endif()
