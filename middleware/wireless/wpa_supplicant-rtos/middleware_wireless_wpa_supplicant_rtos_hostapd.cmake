# Add set(CONFIG_USE_middleware_wireless_wpa_supplicant_rtos_hostapd true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/hostapd/hlr_auc_gw.c
          ${CMAKE_CURRENT_LIST_DIR}/hostapd/ctrl_iface.c
          ${CMAKE_CURRENT_LIST_DIR}/hostapd/eap_register.c
          ${CMAKE_CURRENT_LIST_DIR}/src/ap/dpp_hostapd.c
          ${CMAKE_CURRENT_LIST_DIR}/src/ap/acs.c
          ${CMAKE_CURRENT_LIST_DIR}/src/ap/eth_p_oui.c
          ${CMAKE_CURRENT_LIST_DIR}/src/ap/gas_serv.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/.
          ${CMAKE_CURRENT_LIST_DIR}/src
          ${CMAKE_CURRENT_LIST_DIR}/src/ap
          ${CMAKE_CURRENT_LIST_DIR}/hostapd
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DCONFIG_HOSTAPD=1
              )
  
            if(CONFIG_TOOLCHAIN STREQUAL mcux)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -fomit-frame-pointer
            )
      endif()
      
  endif()

