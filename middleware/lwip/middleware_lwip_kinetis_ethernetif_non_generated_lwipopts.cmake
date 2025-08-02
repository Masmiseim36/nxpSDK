# Add set(CONFIG_USE_middleware_lwip_kinetis_ethernetif_non_generated_lwipopts true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

  if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DLWIP_DISABLE_PBUF_POOL_SIZE_SANITY_CHECKS=1
                        -DLWIP_SUPPORT_CUSTOM_PBUF=1
                        -DCHECKSUM_GEN_UDP=1
                        -DCHECKSUM_GEN_TCP=1
                        -DCHECKSUM_GEN_ICMP=1
                        -DCHECKSUM_GEN_ICMP6=1
                        -DCHECKSUM_CHECK_UDP=1
                        -DCHECKSUM_CHECK_TCP=1
                        -DCHECKSUM_CHECK_ICMP=1
                        -DCHECKSUM_CHECK_ICMP6=1
              )
  
  
  endif()

