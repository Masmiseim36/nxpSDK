if(NOT MIDDLEWARE_EIQ_GLOW_MIMXRT1062_INCLUDED)
    
    set(MIDDLEWARE_EIQ_GLOW_MIMXRT1062_INCLUDED true CACHE BOOL "middleware_eiq_glow component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/glow/bundle_utils/glow_bundle_utils.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/glow/bundle_utils
    )


endif()
