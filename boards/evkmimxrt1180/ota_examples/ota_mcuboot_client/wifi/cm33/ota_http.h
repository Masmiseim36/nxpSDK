/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _OTA_HTTP_H_
#define _OTA_HTTP_H_

#include "core_http_client.h"

struct OtaHttpConf
{
    /* Communication channel interface */
    const TransportInterface_t *ti;

    /* Buffer used for both request headers and response.
     * Must be large enough to hold requested chunk of data
     * together with the response headers.
     */
    void *dataBuf;
    size_t dataBufSize;

    /* Host name as appeared in the HTTP request header*/
    const char *hostName;
};

int OtaHttp_FileRequestInit(const char *fPath,
                            HTTPRequestInfo_t *request,
                            HTTPRequestHeaders_t *requestHeaders,
                            HTTPResponse_t *response,
                            const struct OtaHttpConf *cfg);

int OtaHttp_GetFileContent(
    const char *fPath, uint32_t offset, uint32_t size, void *data, const struct OtaHttpConf *cfg);

int OtaHttp_GetFileSize(const char *fPath, uint32_t *fSize, const struct OtaHttpConf *cfg);

#endif /* _OTA_HTTP_H_ */
