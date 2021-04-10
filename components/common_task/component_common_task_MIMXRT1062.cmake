if(NOT COMPONENT_COMMON_TASK_MIMXRT1062_INCLUDED)
    
    set(COMPONENT_COMMON_TASK_MIMXRT1062_INCLUDED true CACHE BOOL "component_common_task component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_common_task.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

    include(component_osa_MIMXRT1062)

endif()
