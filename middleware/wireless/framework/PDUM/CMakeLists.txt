if(CONNFWK_PDUM_FROM_SOURCES)
    add_library(connfwk-PDUM
        Source/pdum.c
        Source/pdum_apl.c
        Source/pdum_nwk.c
        Source/pdum_nwk_inl.c
        Source/pdum_dbg.c
        Source/pdum_version_lc.S
    )

    add_custom_command(TARGET connfwk-PDUM POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:connfwk-PDUM> ${CMAKE_CURRENT_LIST_DIR}/Library/${CONNFWK_PLATFORM}/libPDUM.a
    )

    target_compile_definitions(connfwk-PDUM
        PRIVATE
            JENNIC_CHIP=${CONNFWK_JENNIC_CHIP}
            JENNIC_CHIP_${CONNFWK_JENNIC_CHIP}
            JENNIC_CHIP_NAME=_${CONNFWK_JENNIC_CHIP}
            JENNIC_CHIP_FAMILY=${CONNFWK_JENNIC_CHIP_FAMILY}
            JENNIC_CHIP_FAMILY_${CONNFWK_JENNIC_CHIP_FAMILY}
            JENNIC_CHIP_FAMILY_NAME=_${CONNFWK_JENNIC_CHIP_FAMILY}
            ${CONNFWK_JENNIC_CHIP}=5189
            ${CONNFWK_JENNIC_CHIP_FAMILY}=5189
            JENNIC_STACK_${CONNFWK_JENNIC_STACK}
            JENNIC_MAC_${CONNFWK_JENNIC_MAC}
    )

    target_include_directories(connfwk-PDUM
        PUBLIC
            Include
    )

    # Get common configs from the connfwk-config interface
    target_link_libraries(connfwk-PDUM
        PRIVATE
            connfwk-config
            ${CONNFWK_MCUX_SDK_LIB}
    )
else()
    add_library(connfwk-PDUM INTERFACE)

    target_include_directories(connfwk-PDUM
        INTERFACE
            Include
    )

    target_link_libraries(connfwk-PDUM
        INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/Library/${CONNFWK_PLATFORM}/libPDUM.a
    )
endif()
