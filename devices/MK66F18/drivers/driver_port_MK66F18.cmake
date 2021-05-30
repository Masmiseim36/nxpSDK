if(NOT DRIVER_PORT_MK66F18_INCLUDED)
    
    set(DRIVER_PORT_MK66F18_INCLUDED true CACHE BOOL "driver_port component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_MK66F18)

endif()
