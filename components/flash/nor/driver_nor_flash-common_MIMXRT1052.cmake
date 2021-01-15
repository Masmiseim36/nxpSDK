if(NOT DRIVER_NOR_FLASH-COMMON_MIMXRT1052_INCLUDED)
    
    set(DRIVER_NOR_FLASH-COMMON_MIMXRT1052_INCLUDED true CACHE BOOL "driver_nor_flash-common component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

endif()
