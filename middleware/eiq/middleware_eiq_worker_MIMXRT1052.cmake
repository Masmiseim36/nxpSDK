if(NOT MIDDLEWARE_EIQ_WORKER_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_EIQ_WORKER_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_eiq_worker component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/common
    )

endif()
