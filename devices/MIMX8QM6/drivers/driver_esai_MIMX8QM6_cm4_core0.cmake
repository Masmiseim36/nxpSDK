if(NOT DRIVER_ESAI_MIMX8QM6_cm4_core0_INCLUDED)
    
    set(DRIVER_ESAI_MIMX8QM6_cm4_core0_INCLUDED true CACHE BOOL "driver_esai component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/fsl_esai.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )


endif()
