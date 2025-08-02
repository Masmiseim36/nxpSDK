# This is NXP version of freemodbus CMakeLists.txt, the freemodbus upstream version is not used.

if (CONFIG_MCUX_COMPONENT_middleware.freemodbus)
    mcux_add_include(
        PROJECT_BASE_PATH freemodbus
        INCLUDES modbus
    )

endif()

if (CONFIG_MCUX_COMPONENT_middleware.freemodbus.rtu)
    mcux_add_include(
        INCLUDES modbus
        INCLUDES modbus/functions
        INCLUDES modbus/include
        INCLUDES modbus/rtu
        INCLUDES modbus/ascii
    )
    mcux_add_source(
        SOURCES
            modbus/mb.c
            modbus/functions/mbfunccoils.c
            modbus/functions/mbfuncdiag.c
            modbus/functions/mbfuncdisc.c
            modbus/functions/mbfuncholding.c
            modbus/functions/mbfuncinput.c
            modbus/functions/mbfuncled.c
            modbus/functions/mbfuncother.c
            modbus/functions/mbutils.c
            modbus/include/mbconfig.h
            modbus/include/mbframe.h
            modbus/include/mbfunc.h
            modbus/include/mb.h
            modbus/include/mbport.h
            modbus/include/mbproto.h
            modbus/include/mbutils.h
            modbus/rtu/mbcrc.c
            modbus/rtu/mbcrc.h
            modbus/rtu/mbrtu.c
            modbus/rtu/mbrtu.h
            modbus/ascii/mbascii.c
            modbus/ascii/mbascii.h
    )
endif ()

if (CONFIG_MCUX_COMPONENT_middleware.freemodbus.rtuc)
    mcux_add_include(
        INCLUDES modbus
        INCLUDES modbus/functions
        INCLUDES modbus/include
        INCLUDES modbus/rtu
        INCLUDES modbus/ascii
    )
    mcux_add_source(
        SOURCES
            modbus/mb_m.c
            modbus/functions/mbfunccoils_m.c
            modbus/functions/mbfuncdisc_m.c
            modbus/functions/mbfuncholding_m.c
            modbus/functions/mbfuncinput_m.c
            modbus/functions/mbfuncled_m.c
            modbus/functions/mbfuncother.c
            modbus/functions/mbutils.c
            modbus/include/mbconfig.h
            modbus/include/mbframe.h
            modbus/include/mbfunc.h
            modbus/include/mb.h
            modbus/include/mb_m.h
            modbus/include/mbport.h
            modbus/include/mbproto.h
            modbus/include/mbutils.h
            modbus/rtu/mbcrc.c
            modbus/rtu/mbcrc.h
            modbus/rtu/mbrtu_m.c
            modbus/rtu/mbrtu.h
            modbus/ascii/mbascii.c
            modbus/ascii/mbascii.h
    )
endif ()

if (CONFIG_MCUX_COMPONENT_middleware.freemodbus.tcp)
    mcux_add_include(
        INCLUDES modbus
        INCLUDES modbus/functions
        INCLUDES modbus/include
        INCLUDES modbus/tcp
    )
    mcux_add_source(
        SOURCES
            modbus/mb.c
            modbus/functions/mbfunccoils.c
            modbus/functions/mbfuncdiag.c
            modbus/functions/mbfuncdisc.c
            modbus/functions/mbfuncholding.c
            modbus/functions/mbfuncinput.c
            modbus/functions/mbfuncled.c
            modbus/functions/mbfuncother.c
            modbus/functions/mbutils.c
            modbus/include/mbconfig.h
            modbus/include/mbframe.h
            modbus/include/mbfunc.h
            modbus/include/mb.h
            modbus/include/mbport.h
            modbus/include/mbproto.h
            modbus/include/mbutils.h
            modbus/tcp/mbtcp.c
            modbus/tcp/mbtcp.h
    )
endif ()

