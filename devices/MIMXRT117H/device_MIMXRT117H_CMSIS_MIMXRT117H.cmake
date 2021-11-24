if(NOT DEVICE_MIMXRT117H_CMSIS_MIMXRT117H_INCLUDED)
    
    set(DEVICE_MIMXRT117H_CMSIS_MIMXRT117H_INCLUDED true CACHE BOOL "device_MIMXRT117H_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    #OR Logic component
    if(CONFIG_USE_CMSIS_Include_core_cm4_MIMXRT117H)
         include(CMSIS_Include_core_cm4_MIMXRT117H)
    endif()
    if(CONFIG_USE_CMSIS_Include_core_cm7_MIMXRT117H)
         include(CMSIS_Include_core_cm7_MIMXRT117H)
    endif()
    if(NOT (CONFIG_USE_CMSIS_Include_core_cm4_MIMXRT117H OR CONFIG_USE_CMSIS_Include_core_cm7_MIMXRT117H))
        message(WARNING "Since CMSIS_Include_core_cm4_MIMXRT117H/CMSIS_Include_core_cm7_MIMXRT117H is not included at first or config in config.cmake file, use CMSIS_Include_core_cm7_MIMXRT117H by default.")
        include(CMSIS_Include_core_cm7_MIMXRT117H)
    endif()

endif()
