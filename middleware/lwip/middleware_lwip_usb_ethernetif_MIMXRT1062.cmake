include_guard(GLOBAL)
message("middleware_lwip_usb_ethernetif component is included.")

if(CONFIG_USE_middleware_baremetal_MIMXRT1062)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/usb_ethernetif_bm.c
)
elseif(CONFIG_USE_middleware_freertos-kernel_MIMXRT1062)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port/usb_ethernetif_freertos.c
)
else()
    message(WARNING "please config middleware.baremetal_MIMXRT1062 or middleware.freertos-kernel_MIMXRT1062 first.")
endif()


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/port
)


#OR Logic component
if(CONFIG_USE_middleware_usb_host_khci_MIMXRT1062)
     include(middleware_usb_host_khci_MIMXRT1062)
endif()
if(CONFIG_USE_middleware_usb_host_ehci_MIMXRT1062)
     include(middleware_usb_host_ehci_MIMXRT1062)
endif()
if(CONFIG_USE_middleware_usb_host_ohci_MIMXRT1062)
     include(middleware_usb_host_ohci_MIMXRT1062)
endif()
if(NOT (CONFIG_USE_middleware_usb_host_khci_MIMXRT1062 OR CONFIG_USE_middleware_usb_host_ehci_MIMXRT1062 OR CONFIG_USE_middleware_usb_host_ohci_MIMXRT1062))
    message(WARNING "Since middleware_usb_host_khci_MIMXRT1062/middleware_usb_host_ehci_MIMXRT1062/middleware_usb_host_ohci_MIMXRT1062 is not included at first or config in config.cmake file, use middleware_usb_host_ehci_MIMXRT1062 by default.")
    include(middleware_usb_host_ehci_MIMXRT1062)
endif()

include(middleware_lwip_MIMXRT1062)

include(middleware_usb_host_cdc_MIMXRT1062)

include(middleware_usb_host_cdc_rndis_MIMXRT1062)

