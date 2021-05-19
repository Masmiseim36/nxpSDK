if(NOT MIDDLEWARE_EIQ_TENSORFLOW_LITE_THIRD_PARTY_RUY_MIMXRT1166_cm7_INCLUDED)
    
    set(MIDDLEWARE_EIQ_TENSORFLOW_LITE_THIRD_PARTY_RUY_MIMXRT1166_cm7_INCLUDED true CACHE BOOL "middleware_eiq_tensorflow_lite_third_party_ruy component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/trmul.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/pack_avx2.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/cpuinfo.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/have_built_path_for_sse42.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/kernel_avxvnni.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/kernel_arm64.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/blocking_counter.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/have_built_path_for_avxvnni.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/allocator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/system_aligned_alloc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/pack_avx512.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/tune.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/context_get_ctx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/kernel_sse42.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/apply_multiplier.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/kernel_avx512.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/kernel_avx2.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/thread_pool.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/prepacked_cache.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/ctx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/context.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/have_built_path_for_avx512.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/have_built_path_for_avx2.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/pack_sse42.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/block_map.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/kernel_arm32.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/pack_avxvnni.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/pack_arm.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/profiler/profiler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/profiler/treeview.cpp
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party/ruy/profiler/instrumentation.cpp
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/tensorflow-lite/third_party
    )


endif()
