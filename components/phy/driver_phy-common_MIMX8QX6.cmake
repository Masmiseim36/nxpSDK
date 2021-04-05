if(NOT DRIVER_PHY-COMMON_MIMX8QX6_INCLUDED)
    
    set(DRIVER_PHY-COMMON_MIMX8QX6_INCLUDED true CACHE BOOL "driver_phy-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_mdio-common_MIMX8QX6)

endif()
