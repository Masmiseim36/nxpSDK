# Copyright 2024 NXP
# SPDX-License-Identifier: BSD-3-Clause

if(CONFIG_MCUX_COMPONENT_middleware.mmcau.cm4_cm7)
    mcux_component_version(2.0.4)
    mcux_add_library(
        LIBS asm-cm4-cm7/lib_mmcau.a
        TOOLCHAINS armgcc mcux iar
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_library(
        LIBS asm-cm4-cm7/lib_mmcau.lib
        TOOLCHAINS mdk
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_include(
        INCLUDES .
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mmcau.cm0p)
    mcux_component_version(2.0.4)
    mcux_add_library(
        LIBS asm-cm0p/lib_mmcau-cm0p.a
        TOOLCHAINS armgcc mcux iar
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_library(
        LIBS asm-cm0p/lib_mmcau-cm0p.lib
        TOOLCHAINS mdk
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_include(
        INCLUDES .
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mmcau.common_files)
    mcux_component_version(2.0.4)
    mcux_add_source(
        SOURCES cau_api.h
                fsl_mmcau.h
                fsl_mmcau.c
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_include(
        INCLUDES .
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.mmcau.mmcau_files)
    mcux_component_version(2.0.4)
    mcux_add_source(
        SOURCES *.txt
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_source(
        SOURCES asm-cm0p/src/*.hdr
        CORES cm0p
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_source(
        SOURCES asm-cm0p/src/*.s
        CORES cm0p
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_source(
        SOURCES asm-cm4-cm7/src/*.hdr
        CORES cm4 cm4f cm7f
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
    mcux_add_source(
        SOURCES asm-cm4-cm7/src/*.s
        CORES cm4 cm4f cm7f
        BASE_PATH ${SdkRootDirPath}/middleware/mmcau/
    )
endif()
