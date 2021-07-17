include_guard(GLOBAL)
message("middleware_usb_host_stack component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_hci.c
    ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_devices.c
    ${CMAKE_CURRENT_LIST_DIR}/host/usb_host_framework.c
    ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_hub.c
    ${CMAKE_CURRENT_LIST_DIR}/host/class/usb_host_hub_app.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/host
    ${CMAKE_CURRENT_LIST_DIR}/host/class
    ${CMAKE_CURRENT_LIST_DIR}/include
)


#OR Logic component
if(CONFIG_USE_middleware_usb_host_khci_MIMXRT1176_cm4)
     include(middleware_usb_host_khci_MIMXRT1176_cm4)
endif()
if(CONFIG_USE_middleware_usb_host_ehci_MIMXRT1176_cm4)
     include(middleware_usb_host_ehci_MIMXRT1176_cm4)
endif()
if(CONFIG_USE_middleware_usb_host_ohci_MIMXRT1176_cm4)
     include(middleware_usb_host_ohci_MIMXRT1176_cm4)
endif()
if(CONFIG_USE_middleware_usb_host_ip3516hs_MIMXRT1176_cm4)
     include(middleware_usb_host_ip3516hs_MIMXRT1176_cm4)
endif()
if(NOT (CONFIG_USE_middleware_usb_host_khci_MIMXRT1176_cm4 OR CONFIG_USE_middleware_usb_host_ehci_MIMXRT1176_cm4 OR CONFIG_USE_middleware_usb_host_ohci_MIMXRT1176_cm4 OR CONFIG_USE_middleware_usb_host_ip3516hs_MIMXRT1176_cm4))
    message(WARNING "Since middleware_usb_host_khci_MIMXRT1176_cm4/middleware_usb_host_ehci_MIMXRT1176_cm4/middleware_usb_host_ohci_MIMXRT1176_cm4/middleware_usb_host_ip3516hs_MIMXRT1176_cm4 is not included at first or config in config.cmake file, use middleware_usb_host_khci_MIMXRT1176_cm4 by default.")
    include(middleware_usb_host_khci_MIMXRT1176_cm4)
endif()

include(component_osa_MIMXRT1176_cm4)

