if(NOT CMSIS_DRIVER_INCLUDE_USART_MKV11Z7_INCLUDED)
    
    set(CMSIS_DRIVER_INCLUDE_USART_MKV11Z7_INCLUDED true CACHE BOOL "CMSIS_Driver_Include_USART component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Driver_Include_Common_MKV11Z7)

endif()
