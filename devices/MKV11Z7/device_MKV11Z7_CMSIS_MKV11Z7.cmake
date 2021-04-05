if(NOT DEVICE_MKV11Z7_CMSIS_MKV11Z7_INCLUDED)
    
    set(DEVICE_MKV11Z7_CMSIS_MKV11Z7_INCLUDED true CACHE BOOL "device_MKV11Z7_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_core_cm0plus_MKV11Z7)

endif()
