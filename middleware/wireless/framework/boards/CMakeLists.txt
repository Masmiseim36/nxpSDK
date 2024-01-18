if(CONNFWK_BOARD)
    message(STATUS "CONNFWK_BOARD: " ${CONNFWK_BOARD})

    if(NOT CONNFWK_FREERTOS_CONFIG_PATH)
        set(CONNFWK_FREERTOS_CONFIG_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${CONNFWK_BOARD}/freertos)
    endif()
    set(CONNFWK_BOARD_LIB connfwk-board-${CONNFWK_BOARD})

    add_subdirectory(${CONNFWK_BOARD})
    target_link_libraries(connfwk-config
        INTERFACE
            ${CONNFWK_BOARD_LIB}
    )
endif()
