# This is NXP version of soem CMakeLists.txt, the soem upstream version is not used.

if (CONFIG_MCUX_COMPONENT_middleware.soem)
    mcux_add_include(
        PROJECT_BASE_PATH soem
        INCLUDES osal
                 oshw/mcux-sdk
                 soem
    )

endif()

if (CONFIG_MCUX_COMPONENT_middleware.soem.enet)
    mcux_add_include(
        INCLUDES oshw/mcux-sdk
        INCLUDES oshw/mcux-sdk/enet
    )
    mcux_add_source(
        SOURCES
            oshw/mcux-sdk/nicdrv.c
            oshw/mcux-sdk/enet/soem_enet.h
            oshw/mcux-sdk/enet/enet.c
            oshw/mcux-sdk/enet/enet.h
    )
endif ()

if (CONFIG_MCUX_COMPONENT_middleware.soem.netc)
    mcux_add_include(
        INCLUDES oshw/mcux-sdk
        INCLUDES oshw/mcux-sdk/netc_ep
    )
    mcux_add_source(
        SOURCES
            oshw/mcux-sdk/nicdrv.c
            oshw/mcux-sdk/netc_ep/soem_netc_ep.h
            oshw/mcux-sdk/netc_ep/netc_ep.c
            oshw/mcux-sdk/netc_ep/netc_ep.h
    )
endif ()

if (CONFIG_MCUX_COMPONENT_middleware.soem.bm)
    mcux_add_include(
        INCLUDES soem
        INCLUDES oshw/mcux-sdk
        INCLUDES osal
        INCLUDES osal/baremetal
    )
    mcux_add_source(
        SOURCES
            soem/ethercat.h
            soem/ethercatbase.c
            soem/ethercatbase.h
            soem/ethercatcoe.c
            soem/ethercatcoe.h
            soem/ethercatconfig.c
            soem/ethercatconfig.h
            soem/ethercatconfiglist.h
            soem/ethercatdc.c
            soem/ethercatdc.h
            soem/ethercateoe.c
            soem/ethercateoe.h
            soem/ethercatfoe.c
            soem/ethercatfoe.h
            soem/ethercatmain.c
            soem/ethercatmain.h
            soem/ethercatprint.c
            soem/ethercatprint.h
            soem/ethercatsoe.c
            soem/ethercatsoe.h
            soem/ethercattype.h
            oshw/mcux-sdk/nicdrv.h
            oshw/mcux-sdk/oshw.c
            oshw/mcux-sdk/oshw.h
            oshw/mcux-sdk/soem_port.c
            oshw/mcux-sdk/soem_port.h
            osal/osal.h
            osal/baremetal/osal.c
            osal/baremetal/osal_defs.h
    )
endif ()

if (CONFIG_MCUX_COMPONENT_middleware.soem.freertos)
    mcux_add_include(
        INCLUDES soem
        INCLUDES oshw/mcux-sdk
        INCLUDES osal
        INCLUDES osal/freertos
    )
    mcux_add_source(
        SOURCES
            soem/ethercat.h
            soem/ethercatbase.c
            soem/ethercatbase.h
            soem/ethercatcoe.c
            soem/ethercatcoe.h
            soem/ethercatconfig.c
            soem/ethercatconfig.h
            soem/ethercatconfiglist.h
            soem/ethercatdc.c
            soem/ethercatdc.h
            soem/ethercateoe.c
            soem/ethercateoe.h
            soem/ethercatfoe.c
            soem/ethercatfoe.h
            soem/ethercatmain.c
            soem/ethercatmain.h
            soem/ethercatprint.c
            soem/ethercatprint.h
            soem/ethercatsoe.c
            soem/ethercatsoe.h
            soem/ethercattype.h
            oshw/mcux-sdk/nicdrv.h
            oshw/mcux-sdk/oshw.c
            oshw/mcux-sdk/oshw.h
            oshw/mcux-sdk/soem_port.c
            oshw/mcux-sdk/soem_port.h
            osal/osal.h
            osal/freertos/osal.c
            osal/freertos/osal_defs.h
    )
endif ()
