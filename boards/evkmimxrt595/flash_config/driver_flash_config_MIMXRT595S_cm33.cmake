if(NOT DRIVER_FLASH_CONFIG_MIMXRT595S_cm33_INCLUDED)
    
    set(DRIVER_FLASH_CONFIG_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "driver_flash_config component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/flash_config.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_iap_MIMXRT595S_cm33)

endif()
