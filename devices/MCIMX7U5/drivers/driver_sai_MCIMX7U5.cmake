if(NOT DRIVER_SAI_MCIMX7U5_INCLUDED)
    
    set(DRIVER_SAI_MCIMX7U5_INCLUDED true CACHE BOOL "driver_sai component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_sai.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


    include(driver_common_MCIMX7U5)

endif()
