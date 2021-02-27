if(NOT COMPONENT_LOG_BACKEND_RINGBUFFER_MIMXRT595S_cm33_INCLUDED)
    
    set(COMPONENT_LOG_BACKEND_RINGBUFFER_MIMXRT595S_cm33_INCLUDED true CACHE BOOL "component_log_backend_ringbuffer component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_log_backend_ringbuffer.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT595S_cm33)

    include(component_log_MIMXRT595S_cm33)

endif()
