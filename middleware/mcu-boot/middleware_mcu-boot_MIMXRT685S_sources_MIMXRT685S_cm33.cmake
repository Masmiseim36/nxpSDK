if(NOT MIDDLEWARE_MCU-BOOT_MIMXRT685S_SOURCES_MIMXRT685S_cm33_INCLUDED)
    
    set(MIDDLEWARE_MCU-BOOT_MIMXRT685S_SOURCES_MIMXRT685S_cm33_INCLUDED true CACHE BOOL "middleware_mcu-boot_MIMXRT685S_sources component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/bl_clock_config_MIMXRT685S.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/hardware_init_MIMXRT685S.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/memory_map_MIMXRT685S.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/external_memory_property_map_MIMXRT685S.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/bl_api.c
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src/bl_main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_context.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_exception_handler.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_tree_root.c
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader/src/bl_app_crc_check.c
        ${CMAKE_CURRENT_LIST_DIR}/src/crc/src/crc16.c
        ${CMAKE_CURRENT_LIST_DIR}/src/crc/src/crc32.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/memory.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/normal_memory.c
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src/pattern_fill_gcc.S
        ${CMAKE_CURRENT_LIST_DIR}/src/utilities/src/fsl_assert.c
        ${CMAKE_CURRENT_LIST_DIR}/src/utilities/src/fsl_rtos_abstraction.c
        ${CMAKE_CURRENT_LIST_DIR}/src/utilities/src/debug_util.c
        ${CMAKE_CURRENT_LIST_DIR}/src/property/src/property_rt685s.c
        ${CMAKE_CURRENT_LIST_DIR}/src/startup/startup.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/targets/MIMXRT685S/src
        ${CMAKE_CURRENT_LIST_DIR}/components/osa
        ${CMAKE_CURRENT_LIST_DIR}/src/bootloader
        ${CMAKE_CURRENT_LIST_DIR}/src/crc
        ${CMAKE_CURRENT_LIST_DIR}/src/include
        ${CMAKE_CURRENT_LIST_DIR}/src/memory
        ${CMAKE_CURRENT_LIST_DIR}/src/memory/src
        ${CMAKE_CURRENT_LIST_DIR}/src/startup
        ${CMAKE_CURRENT_LIST_DIR}/src/packet
        ${CMAKE_CURRENT_LIST_DIR}/src/property
        ${CMAKE_CURRENT_LIST_DIR}/src/utilities
    )


endif()
