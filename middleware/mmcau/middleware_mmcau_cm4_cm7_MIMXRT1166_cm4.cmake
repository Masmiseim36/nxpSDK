if(NOT MIDDLEWARE_MMCAU_CM4_CM7_MIMXRT1166_cm4_INCLUDED)
    
    set(MIDDLEWARE_MMCAU_CM4_CM7_MIMXRT1166_cm4_INCLUDED true CACHE BOOL "middleware_mmcau_cm4_cm7 component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    include(driver_clock_MIMXRT1166_cm4)

endif()
