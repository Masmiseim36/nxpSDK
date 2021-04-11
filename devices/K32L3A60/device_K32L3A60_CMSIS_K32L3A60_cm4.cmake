if(NOT DEVICE_K32L3A60_CMSIS_K32L3A60_cm4_INCLUDED)
    
    set(DEVICE_K32L3A60_CMSIS_K32L3A60_cm4_INCLUDED true CACHE BOOL "device_K32L3A60_CMSIS component is included.")


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/.
    )

    #OR Logic component
    if(CONFIG_USE_CMSIS_Include_core_cm0plus_K32L3A60_cm4)
         include(CMSIS_Include_core_cm0plus_K32L3A60_cm4)
    endif()
    if(CONFIG_USE_CMSIS_Include_core_cm4_K32L3A60_cm4)
         include(CMSIS_Include_core_cm4_K32L3A60_cm4)
    endif()
    if(NOT (CONFIG_USE_CMSIS_Include_core_cm0plus_K32L3A60_cm4 OR CONFIG_USE_CMSIS_Include_core_cm4_K32L3A60_cm4))
        message(WARNING "Since CMSIS_Include_core_cm0plus_K32L3A60_cm4/CMSIS_Include_core_cm4_K32L3A60_cm4 is not included at first or config in config.cmake file, use CMSIS_Include_core_cm4_K32L3A60_cm4 by default.")
        include(CMSIS_Include_core_cm4_K32L3A60_cm4)
    endif()

endif()
