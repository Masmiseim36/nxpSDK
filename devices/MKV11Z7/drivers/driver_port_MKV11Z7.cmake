if(NOT DRIVER_PORT_MKV11Z7_INCLUDED)
    
    set(DRIVER_PORT_MKV11Z7_INCLUDED true CACHE BOOL "driver_port component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_common_MKV11Z7)

endif()
