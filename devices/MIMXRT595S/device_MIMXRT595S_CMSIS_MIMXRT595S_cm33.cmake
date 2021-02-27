if(NOT DEVICE_MIMXRT595S_CMSIS_MIMXRT595S_cm33_INCLUDED)
    
    set(DEVICE_MIMXRT595S_CMSIS_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "device_MIMXRT595S_CMSIS component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/system_MIMXRT595S_cm33.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(CMSIS_Include_core_cm33_MIMXRT595S_cm33)

endif()
