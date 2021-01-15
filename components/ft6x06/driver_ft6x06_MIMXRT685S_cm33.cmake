if(NOT DRIVER_FT6X06_MIMXRT685S_cm33_INCLUDED)
    
    set(DRIVER_FT6X06_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "driver_ft6x06 component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_ft6x06.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(CMSIS_Driver_Include_I2C_MIMXRT685S_cm33)

endif()
