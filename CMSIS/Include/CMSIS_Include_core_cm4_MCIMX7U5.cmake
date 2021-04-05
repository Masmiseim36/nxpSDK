if(NOT CMSIS_INCLUDE_CORE_CM4_MCIMX7U5_INCLUDED)
    
    set(CMSIS_INCLUDE_CORE_CM4_MCIMX7U5_INCLUDED true CACHE BOOL "CMSIS_Include_core_cm4 component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_common_MCIMX7U5)

    include(CMSIS_Include_dsp_MCIMX7U5)

endif()
