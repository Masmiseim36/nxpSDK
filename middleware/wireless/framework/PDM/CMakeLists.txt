add_library(connfwk-PDM INTERFACE)

target_include_directories(connfwk-PDM
    INTERFACE
        Include
)

target_link_libraries(connfwk-PDM
    INTERFACE
        connfwk-config
)
