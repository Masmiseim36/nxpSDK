if(NOT MIDDLEWARE_SDMMC_OSA_BM_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_SDMMC_OSA_BM_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_sdmmc_osa_bm component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/osa/fsl_sdmmc_osa.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/osa
    )


    include(component_osa_bm_MIMXRT1062)

endif()
