# Add set(CONFIG_USE_middleware_cadence_multicore_xaf true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/rbtree.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xa-class-audio-codec.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xa-class-base.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xa-class-capturer.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xa-class-mimo-proc.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xa-class-mixer.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xa-class-renderer.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-core.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-io.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-ipc-if.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-ipi.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-isr.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-main.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-mem-ipc.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-mem.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-msg.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-msgq.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/src/xf-sched.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/host-apf/src/xaf-api.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/host-apf/src/xf-msgq1.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/host-apf/src/xf-proxy.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/host-apf/src/xf-trace.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/xa-factory.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/aac_dec/xa-aac-decoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/aec22/xa-aec22.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/aec23/xa-aec23.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/mixer/xa-mixer.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/mp3_dec/xa-mp3-decoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/mp3_enc/xa-mp3-encoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/opus_dec/opus_header.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/opus_dec/xa-opus-decoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/opus_enc/xa-opus-encoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/pcm_gain/xa-pcm-gain.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/pcm_split/xa-pcm-split.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/src-pp/xa-src-pp.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/cadence/vorbis_dec/xa-vorbis-decoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/capturer/xa-capturer-dmic.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/capturer/xa-capturer-i2s.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/capturer/xa-capturer-pdm.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/pcm_stereo/xa-pcm-stereo.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/renderer/xa-renderer-sai.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/renderer/xa-renderer.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/sbc_dec/xa-sbc-decoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/sbc_enc/xa-sbc-encoder.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/vit_pre_proc/vit_pre_proc.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/voice_seeker/voice_seeker.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/src/xaf-fio-test.c
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/src/xaf-mem-test.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/include
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/include/audio
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/include/lib
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/hifi-dpf/include/sys/xos-msgq/iss
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/host-apf/include
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/host-apf/include/sys/xos-msgq
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/algo/xa_af_hostless/include
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/include
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/include/audio
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/include/sysdeps/xos/include
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/include/sysdeps/mc_ipc
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/include
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/include/audio
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/vit_pre_proc
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/test/plugins/nxp/voice_seeker
          ${CMAKE_CURRENT_LIST_DIR}/xa_af_hostless/xf_shared/include
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

  
            if(CONFIG_TOOLCHAIN STREQUAL xcc)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -mno-mul16
              -mno-mul32
              -mno-div32
              -fsigned-char
              -mlongcalls
              -INLINE:requested
            )
      endif()
          if(CONFIG_TOOLCHAIN STREQUAL xtensa)
      target_compile_options(${MCUX_SDK_PROJECT_NAME} PUBLIC
              -mno-mul16
              -mno-mul32
              -mno-div32
              -fsigned-char
              -mlongcalls
              -INLINE:requested
            )
      endif()
      
  endif()

