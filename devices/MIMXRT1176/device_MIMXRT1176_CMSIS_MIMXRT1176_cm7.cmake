if(NOT DEVICE_MIMXRT1176_CMSIS_MIMXRT1176_cm7_INCLUDED)
    
    set(DEVICE_MIMXRT1176_CMSIS_MIMXRT1176_cm7_INCLUDED true CACHE BOOL "device_MIMXRT1176_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    #OR Logic component
    if(CONFIG_USE_CMSIS_Include_core_cm4_MIMXRT1176_cm7)
         include(CMSIS_Include_core_cm4_MIMXRT1176_cm7)
    endif()
    if(CONFIG_USE_CMSIS_Include_core_cm7_MIMXRT1176_cm7)
         include(CMSIS_Include_core_cm7_MIMXRT1176_cm7)
    endif()
    if(NOT (CONFIG_USE_CMSIS_Include_core_cm4_MIMXRT1176_cm7 OR CONFIG_USE_CMSIS_Include_core_cm7_MIMXRT1176_cm7))
        message(WARNING "Since CMSIS_Include_core_cm4_MIMXRT1176_cm7/CMSIS_Include_core_cm7_MIMXRT1176_cm7 is not included at first or config in config.cmake file, use CMSIS_Include_core_cm7_MIMXRT1176_cm7 by default.")
        include(CMSIS_Include_core_cm7_MIMXRT1176_cm7)
    endif()

endif()
