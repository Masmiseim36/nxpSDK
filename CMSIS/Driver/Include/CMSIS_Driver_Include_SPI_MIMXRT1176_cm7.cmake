if(NOT CMSIS_DRIVER_INCLUDE_SPI_MIMXRT1176_cm7_INCLUDED)
    
    set(CMSIS_DRIVER_INCLUDE_SPI_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "CMSIS_Driver_Include_SPI component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Driver_Include_Common_MIMXRT1176_cm7)

endif()
