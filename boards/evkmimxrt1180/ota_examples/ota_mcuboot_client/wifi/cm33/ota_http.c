/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <assert.h>

#include "ota_http.h"

#define HTTP_STATUS_PARTIAL_CONTENT   206
#define HTTP_HEADER_CONTENT_RANGE     "Content-Range"
#define HTTP_HEADER_CONTENT_RANGE_LEN (sizeof(HTTP_HEADER_CONTENT_RANGE) - 1)

int OtaHttp_FileRequestInit(const char *fPath,
                            HTTPRequestInfo_t *request,
                            HTTPRequestHeaders_t *requestHeaders,
                            HTTPResponse_t *response,
                            const struct OtaHttpConf *cfg)
{
    if (!(fPath && request && requestHeaders && response && cfg))
    {
        return HTTPInvalidParameter;
    }

    memset(response, 0, sizeof(*response));
    memset(request, 0, sizeof(*request));
    memset(requestHeaders, 0, sizeof(*requestHeaders));

    request->pPath     = fPath;
    request->pathLen   = strlen(fPath);
    request->pHost     = cfg->hostName;
    request->hostLen   = strlen(cfg->hostName);
    request->pMethod   = HTTP_METHOD_GET;
    request->methodLen = sizeof(HTTP_METHOD_GET) - 1;

    /* Multiple requests during single connection.
     * That means HTTP1.1 is expected, HTTP1.0 won't work!
     */
    request->reqFlags = HTTP_REQUEST_KEEP_ALIVE_FLAG;

    /* Request and Response use the same buffer */
    requestHeaders->pBuffer   = cfg->dataBuf;
    requestHeaders->bufferLen = cfg->dataBufSize;
    response->pBuffer         = cfg->dataBuf;
    response->bufferLen       = cfg->dataBufSize;

    return HTTPClient_InitializeRequestHeaders(requestHeaders, request);
}

int OtaHttp_GetFileContent(const char *fPath, uint32_t offset, uint32_t size, void *data, const struct OtaHttpConf *cfg)
{
    HTTPResponse_t response;
    HTTPRequestInfo_t request;
    HTTPRequestHeaders_t requestHeaders;
    int ret;

    if (!(fPath && data && cfg))
    {
        return -HTTPInvalidParameter;
    }

    if (size == 0)
        return 0;

    /* Buffer used for entire HTTP response must be larger
     * than requested size due to extra size for HTTP headers.
     */

    if (cfg->dataBufSize <= size)
    {
        return -HTTPInsufficientMemory;
    }

    ret = OtaHttp_FileRequestInit(fPath, &request, &requestHeaders, &response, cfg);
    if (ret != HTTPSuccess)
    {
        return -ret;
    }

    ret = HTTPClient_AddRangeHeader(&requestHeaders, offset, offset + size - 1);
    if (ret != HTTPSuccess)
    {
        return -ret;
    }

    ret = HTTPClient_Send(cfg->ti, &requestHeaders, NULL, 0, &response, 0);

    /* Should this hold? */
    /* assert(response.bodyLen == response.contentLength); */

    if (ret != HTTPSuccess)
    {
        return -ret;
    }
    else if (response.statusCode != HTTP_STATUS_PARTIAL_CONTENT)
    {
        return response.statusCode;
    }
    else if (response.contentLength > size)
    {
        return -HTTPInvalidResponse;
    }

    memcpy(data, response.pBody, response.contentLength);
    return response.contentLength;
}

/* Returns zero on success.
 * Returns negative value of erroneous HTTPStatus.
 * Returns positive value of unexpected HTTP response status code (e.g. 404).
 */

int OtaHttp_GetFileSize(const char *fPath, uint32_t *fSize, const struct OtaHttpConf *cfg)
{
    HTTPRequestInfo_t request;
    HTTPRequestHeaders_t requestHeaders;
    HTTPResponse_t response;
    int ret;

    const char *contentRangeStr = NULL;
    size_t contentRangeStrLen   = 0;

    if (!(fPath && cfg))
    {
        return -HTTPInvalidParameter;
    }

    ret = OtaHttp_FileRequestInit(fPath, &request, &requestHeaders, &response, cfg);
    if (ret != HTTPSuccess)
    {
        return -ret;
    }

    /* Use Partial Content transfer of zero range to get only
     * the file size, which will be specified by a response header
     * as "Content-Range: bytes 0-0/<filesize>"
     */

    ret = HTTPClient_AddRangeHeader(&requestHeaders, 0, 0);
    if (ret != HTTPSuccess)
    {
        return -ret;
    }

    /* Request Send */

    ret = HTTPClient_Send(cfg->ti, &requestHeaders, NULL, 0, &response, 0);
    if (ret != HTTPSuccess)
    {
        return -ret;
    }
    else if (response.statusCode != HTTP_STATUS_PARTIAL_CONTENT)
    {
        return response.statusCode;
    }

    /* Response Inspection */

    ret = HTTPClient_ReadHeader(&response, HTTP_HEADER_CONTENT_RANGE, HTTP_HEADER_CONTENT_RANGE_LEN, &contentRangeStr,
                                &contentRangeStrLen);
    if (ret != HTTPSuccess)
    {
        return -ret;
    }

    /* Content size string conversion */
    {
        char *endptr;
        char *str;
        unsigned long size;

        /* Expected format to parse:
         *    "Content-Range: bytes 0-0/<filesize>"
         */

        str = strchr(contentRangeStr, '/');
        if (str == NULL)
        {
            return -HTTPInvalidResponse;
        }

        str++;
        size = strtoul(str, &endptr, 10);

        /*
         * In case of strtoul errno should be also inspected in case
         * of 0 or ULONG_MAX, however both values are considered
         * unexpected so treat them as error anyway.
         * The size is compared against UINT32_MAX since this is the
         * return type, however, unsigned long and uint32_t should be
         * of equal size.
         */
        if (!isspace((int)*endptr) || size == 0 || size >= UINT32_MAX)
        {
            return -HTTPInvalidResponse;
        }

        *fSize = (uint32_t)size;
    }

    return 0;
}
