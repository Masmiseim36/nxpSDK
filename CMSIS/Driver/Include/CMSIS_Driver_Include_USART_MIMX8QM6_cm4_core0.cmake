if(NOT CMSIS_DRIVER_INCLUDE_USART_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(CMSIS_DRIVER_INCLUDE_USART_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "CMSIS_Driver_Include_USART component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Driver_Include_Common_MIMX8QM6_cm4_core0)

endif()
