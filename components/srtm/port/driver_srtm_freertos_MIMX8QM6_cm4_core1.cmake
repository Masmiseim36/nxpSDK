if(NOT DRIVER_SRTM_FREERTOS_MIMX8QM6_cm4_core1_INCLUDED)
    
    set(DRIVER_SRTM_FREERTOS_MIMX8QM6_cm4_core1_INCLUDED true CACHE BOOL "driver_srtm_freertos component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/srtm_heap_freertos.c
        ${CMAKE_CURRENT_LIST_DIR}/srtm_mutex_freertos.c
        ${CMAKE_CURRENT_LIST_DIR}/srtm_sem_freertos.c
    )


    include(driver_srtm_MIMX8QM6_cm4_core1)

endif()
