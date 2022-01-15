include_guard(GLOBAL)
message("middleware_eiq_worker_audio component is included.")

target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_audio_worker.c
    ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_micro.c
    ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_speaker.c
    ${CMAKE_CURRENT_LIST_DIR}/common/audio/eiq_speaker_conf.c
)


target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/common/audio
)


include(middleware_eiq_worker_MIMXRT1176_cm4)

