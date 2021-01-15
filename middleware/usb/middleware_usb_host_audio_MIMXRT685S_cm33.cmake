if(NOT MIDDLEWARE_USB_HOST_AUDIO_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_USB_HOST_AUDIO_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_usb_host_audio component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_audio.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/host/class
    )


    include(middleware_usb_host_stack_MIMXRT685S_cm33)

endif()
