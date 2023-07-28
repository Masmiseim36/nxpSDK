# Add set(CONFIG_USE_middleware_dsp_audio_streamer_libxa_af_hostless true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/rbtree.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-base.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-core.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-io.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipc-if.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-ipi.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-isr.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-main.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem-ipc.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-mem.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msg.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-msgq1.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/src/xf-sched.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/host-apf/src/xaf-api.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-proxy.c
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/host-apf/src/xf-trace.c
)

target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/include
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/audio
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/lib
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/host-apf/include
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/host-apf/include/sys/xos-msgq
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/algo/xa_af_hostless/include
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/include
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/include/audio
  ${CMAKE_CURRENT_LIST_DIR}/audio_framework/libxa_af_hostless/include/sysdeps/xos/include
)

if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
    -DHAVE_XOS
    -DHIFI_ONLY_XAF
  )

endif()

