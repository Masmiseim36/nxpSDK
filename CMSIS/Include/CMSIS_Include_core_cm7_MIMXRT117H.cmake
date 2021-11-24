if(NOT CMSIS_INCLUDE_CORE_CM7_MIMXRT117H_INCLUDED)
    
    set(CMSIS_INCLUDE_CORE_CM7_MIMXRT117H_INCLUDED true CACHE BOOL "CMSIS_Include_core_cm7 component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(CMSIS_Include_common_MIMXRT117H)

    include(CMSIS_Include_dsp_MIMXRT117H)

endif()
