# Add set(CONFIG_USE_middleware_edgefast_bluetooth_porting true) in config.cmake to use this component

include_guard(GLOBAL)
message("${CMAKE_CURRENT_LIST_FILE} component is included.")

      target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/ring_buffer.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/porting.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/utf8.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/fifo/fifo.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/lifo/lifo.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/poll/poll.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/queue/queue.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/sem/sem.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/slab/slab.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/slist/slist.c
          ${CMAKE_CURRENT_LIST_DIR}/source/porting/thread/thread.c
        )

  
      target_include_directories(${MCUX_SDK_PROJECT_NAME} PUBLIC
          ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/source/porting
        )

    if(CONFIG_USE_COMPONENT_CONFIGURATION)
  message("===>Import configuration from ${CMAKE_CURRENT_LIST_FILE}")

      target_compile_definitions(${MCUX_SDK_PROJECT_NAME} PUBLIC
                  -DLOG_ENABLE_ASYNC_MODE=1
                        -DLOG_MAX_ARGUMENT_COUNT=10
                        -DLOG_ENABLE_OVERWRITE=0
              )
  
  
  endif()

