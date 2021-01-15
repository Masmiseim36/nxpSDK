if(NOT COMPONENT_LISTS_MIMXRT1062_INCLUDED)
    
    set(COMPONENT_LISTS_MIMXRT1062_INCLUDED true CACHE BOOL "component_lists component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_component_generic_list.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MIMXRT1062)

endif()
