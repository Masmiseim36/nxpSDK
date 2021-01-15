if(NOT MIDDLEWARE_AZURE_IOT_MIMXRT1052_INCLUDED)
    
    set(MIDDLEWARE_AZURE_IOT_MIMXRT1052_INCLUDED true CACHE BOOL "middleware_azure_iot component is included.")

    target_sources(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/azure_base32.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/azure_base64.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/buffer.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/connection_string_parser.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/consolelogger.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/constbuffer.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/constmap.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/crt_abstractions.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/doublylinkedlist.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/gballoc.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/hmac_azure.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/hmacsha256.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/http_proxy_io.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/httpapiex.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/httpapiexsas.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/httpheaders.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/map.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/optionhandler.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/sastoken.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/sha1_azure.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/sha224.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/sha384-512.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/singlylinkedlist.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/strings.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/string_tokenizer.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/string_token.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/urlencode.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/usha.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/uuid.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/vector.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/xio.c
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/src/xlogging.c
    )


    target_include_directories(${MCUX_SDK_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/c-utility/inc
    )


endif()
