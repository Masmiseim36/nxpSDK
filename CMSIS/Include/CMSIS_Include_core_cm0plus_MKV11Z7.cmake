if(NOT CMSIS_INCLUDE_CORE_CM0PLUS_MKV11Z7_INCLUDED)
    
    set(CMSIS_INCLUDE_CORE_CM0PLUS_MKV11Z7_INCLUDED true CACHE BOOL "CMSIS_Include_core_cm0plus component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_common_MKV11Z7)

    include(CMSIS_Include_dsp_MKV11Z7)

endif()
