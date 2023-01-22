include_guard()
message("middleware_soem component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercateoe.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatconfig.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatsoe.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatcoe.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatprint.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatfoe.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatmain.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatdc.c
    ${CMAKE_CURRENT_LIST_DIR}/soem/ethercatbase.c
    ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/enet/enet.c
    ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/oshw.c
    ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/soem_port.c
    ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/nicdrv.c
)

if(CONFIG_USE_middleware_baremetal_MIMXRT1176_cm7)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osal/baremetal/osal.c
)
elseif(CONFIG_USE_middleware_freertos-kernel_MIMXRT1176_cm7)
target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/osal/freertos/osal.c
)
else()
    message(WARNING "please config middleware.baremetal_MIMXRT1176_cm7 or middleware.freertos-kernel_MIMXRT1176_cm7 first.")
endif()


target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/soem
    ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk
    ${CMAKE_CURRENT_LIST_DIR}/oshw/mcux-sdk/enet
    ${CMAKE_CURRENT_LIST_DIR}/osal
)


#OR Logic component
if(CONFIG_USE_middleware_baremetal_MIMXRT1176_cm7)
     include(middleware_baremetal_MIMXRT1176_cm7)
endif()
if(CONFIG_USE_middleware_freertos-kernel_MIMXRT1176_cm7)
     include(middleware_freertos-kernel_MIMXRT1176_cm7)
endif()
if(NOT (CONFIG_USE_middleware_baremetal_MIMXRT1176_cm7 OR CONFIG_USE_middleware_freertos-kernel_MIMXRT1176_cm7))
    message(WARNING "Since middleware_baremetal_MIMXRT1176_cm7/middleware_freertos-kernel_MIMXRT1176_cm7 is not included at first or config in config.cmake file, use middleware_freertos-kernel_MIMXRT1176_cm7 by default.")
    include(middleware_freertos-kernel_MIMXRT1176_cm7)
endif()

