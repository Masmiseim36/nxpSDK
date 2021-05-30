if(NOT COMPONENT_LOG_BACKEND_RINGBUFFER_MK66F18_INCLUDED)
    
    set(COMPONENT_LOG_BACKEND_RINGBUFFER_MK66F18_INCLUDED true CACHE BOOL "component_log_backend_ringbuffer component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_log_backend_ringbuffer.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MK66F18)

    include(component_log_MK66F18)

endif()
