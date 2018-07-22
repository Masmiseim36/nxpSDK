/* ocsp.c
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * http://www.wolfssl.com
 */



  /* Name change compatibility layer no longer needs to be included here */

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#ifndef WOLFCRYPT_ONLY
#ifdef HAVE_OCSP

#include <wolfssl/error-ssl.h>
#include <wolfssl/ocsp.h>
#include <wolfssl/internal.h>

#ifdef NO_INLINE
    #include <wolfssl/wolfcrypt/misc.h>
#else
    #define WOLFSSL_MISC_INCLUDED
    #include <wolfcrypt/src/misc.c>
#endif


int InitOCSP(WOLFSSL_OCSP* ocsp, WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("InitOCSP");

    ForceZero(ocsp, sizeof(WOLFSSL_OCSP));

    if (InitMutex(&ocsp->ocspLock) != 0)
        return BAD_MUTEX_E;

    ocsp->cm = cm;

    return 0;
}


static int InitOcspEntry(OcspEntry* entry, OcspRequest* request)
{
    WOLFSSL_ENTER("InitOcspEntry");

    ForceZero(entry, sizeof(OcspEntry));

    XMEMCPY(entry->issuerHash,    request->issuerHash,    OCSP_DIGEST_SIZE);
    XMEMCPY(entry->issuerKeyHash, request->issuerKeyHash, OCSP_DIGEST_SIZE);

    return 0;
}


static void FreeOcspEntry(OcspEntry* entry, void* heap)
{
    CertStatus *status, *next;

    WOLFSSL_ENTER("FreeOcspEntry");

    for (status = entry->status; status; status = next) {
        next = status->next;

        if (status->rawOcspResponse)
            XFREE(status->rawOcspResponse, heap, DYNAMIC_TYPE_OCSP_STATUS);

        XFREE(status, heap, DYNAMIC_TYPE_OCSP_STATUS);
    }

    (void)heap;
}


void FreeOCSP(WOLFSSL_OCSP* ocsp, int dynamic)
{
    OcspEntry *entry, *next;

    WOLFSSL_ENTER("FreeOCSP");

    for (entry = ocsp->ocspList; entry; entry = next) {
        next = entry->next;
        FreeOcspEntry(entry, ocsp->cm->heap);
        XFREE(entry, ocsp->cm->heap, DYNAMIC_TYPE_OCSP_ENTRY);
    }

    FreeMutex(&ocsp->ocspLock);

    if (dynamic)
        XFREE(ocsp, ocsp->cm->heap, DYNAMIC_TYPE_OCSP);

}


static int xstat2err(int stat)
{
    switch (stat) {
        case CERT_GOOD:
            return 0;
        case CERT_REVOKED:
            return OCSP_CERT_REVOKED;
        default:
            return OCSP_CERT_UNKNOWN;
    }
}


int CheckCertOCSP(WOLFSSL_OCSP* ocsp, DecodedCert* cert, buffer* responseBuffer)
{
    int ret = OCSP_LOOKUP_FAIL;

#ifdef WOLFSSL_SMALL_STACK
    OcspRequest* ocspRequest;
#else
    OcspRequest ocspRequest[1];
#endif

    WOLFSSL_ENTER("CheckCertOCSP");


#ifdef WOLFSSL_SMALL_STACK
    ocspRequest = (OcspRequest*)XMALLOC(sizeof(OcspRequest), NULL,
                                                       DYNAMIC_TYPE_TMP_BUFFER);
    if (ocspRequest == NULL) {
        WOLFSSL_LEAVE("CheckCertOCSP", MEMORY_ERROR);
        return MEMORY_E;
    }
#endif

    if (InitOcspRequest(ocspRequest, cert, ocsp->cm->ocspSendNonce,
                                                         ocsp->cm->heap) == 0) {
        ret = CheckOcspRequest(ocsp, ocspRequest, responseBuffer);

        FreeOcspRequest(ocspRequest);
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(ocspRequest, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    WOLFSSL_LEAVE("CheckCertOCSP", ret);
    return ret;
}

static int GetOcspEntry(WOLFSSL_OCSP* ocsp, OcspRequest* request,
                                                              OcspEntry** entry)
{
    WOLFSSL_ENTER("GetOcspEntry");

    *entry = NULL;

    if (LockMutex(&ocsp->ocspLock) != 0) {
        WOLFSSL_LEAVE("CheckCertOCSP", BAD_MUTEX_E);
        return BAD_MUTEX_E;
    }

    for (*entry = ocsp->ocspList; *entry; *entry = (*entry)->next)
        if (XMEMCMP((*entry)->issuerHash,    request->issuerHash,
                                                         OCSP_DIGEST_SIZE) == 0
        &&  XMEMCMP((*entry)->issuerKeyHash, request->issuerKeyHash,
                                                         OCSP_DIGEST_SIZE) == 0)
            break;

    if (*entry == NULL) {
        *entry = (OcspEntry*)XMALLOC(sizeof(OcspEntry),
                                       ocsp->cm->heap, DYNAMIC_TYPE_OCSP_ENTRY);
        if (*entry) {
            InitOcspEntry(*entry, request);
            (*entry)->next = ocsp->ocspList;
            ocsp->ocspList = *entry;
        }
    }

    UnLockMutex(&ocsp->ocspLock);

    return *entry ? 0 : MEMORY_ERROR;
}


static int GetOcspStatus(WOLFSSL_OCSP* ocsp, OcspRequest* request,
                  OcspEntry* entry, CertStatus** status, buffer* responseBuffer)
{
    int ret = OCSP_INVALID_STATUS;

    WOLFSSL_ENTER("GetOcspStatus");

    *status = NULL;

    if (LockMutex(&ocsp->ocspLock) != 0) {
        WOLFSSL_LEAVE("CheckCertOCSP", BAD_MUTEX_E);
        return BAD_MUTEX_E;
    }

    for (*status = entry->status; *status; *status = (*status)->next)
        if ((*status)->serialSz == request->serialSz
        &&  !XMEMCMP((*status)->serial, request->serial, (*status)->serialSz))
            break;

    if (responseBuffer && *status && !(*status)->rawOcspResponse) {
        /* force fetching again */
        ret = OCSP_INVALID_STATUS;
    }
    else if (*status) {
        if (ValidateDate((*status)->thisDate, (*status)->thisDateFormat, BEFORE)
        &&  ((*status)->nextDate[0] != 0)
        &&  ValidateDate((*status)->nextDate, (*status)->nextDateFormat, AFTER))
        {
            ret = xstat2err((*status)->status);

            if (responseBuffer) {
                responseBuffer->buffer = (byte*)XMALLOC(
                   (*status)->rawOcspResponseSz, NULL, DYNAMIC_TYPE_TMP_BUFFER);

                if (responseBuffer->buffer) {
                    responseBuffer->length = (*status)->rawOcspResponseSz;
                    XMEMCPY(responseBuffer->buffer,
                            (*status)->rawOcspResponse,
                            (*status)->rawOcspResponseSz);
                }
            }
        }
    }

    UnLockMutex(&ocsp->ocspLock);

    return ret;
}

int CheckOcspRequest(WOLFSSL_OCSP* ocsp, OcspRequest* ocspRequest,
                                                      buffer* responseBuffer)
{
    OcspEntry*  entry          = NULL;
    CertStatus* status         = NULL;
    byte*       request        = NULL;
    int         requestSz      = 2048;
    byte*       response       = NULL;
    const char* url            = NULL;
    int         urlSz          = 0;
    int         ret            = -1;

#ifdef WOLFSSL_SMALL_STACK
    CertStatus* newStatus;
    OcspResponse* ocspResponse;
#else
    CertStatus newStatus[1];
    OcspResponse ocspResponse[1];
#endif

    WOLFSSL_ENTER("CheckOcspRequest");

    if (responseBuffer) {
        responseBuffer->buffer = NULL;
        responseBuffer->length = 0;
    }

    ret = GetOcspEntry(ocsp, ocspRequest, &entry);
    if (ret != 0)
        return ret;

    ret = GetOcspStatus(ocsp, ocspRequest, entry, &status, responseBuffer);
    if (ret != OCSP_INVALID_STATUS)
        return ret;

    if (ocsp->cm->ocspUseOverrideURL) {
        url = ocsp->cm->ocspOverrideURL;
        if (url != NULL && url[0] != '\0')
            urlSz = (int)XSTRLEN(url);
        else
            return OCSP_NEED_URL;
    }
    else if (ocspRequest->urlSz != 0 && ocspRequest->url != NULL) {
        url = (const char *)ocspRequest->url;
        urlSz = ocspRequest->urlSz;
    }
    else {
        /* cert doesn't have extAuthInfo, assuming CERT_GOOD */
        return 0;
    }

    request = (byte*)XMALLOC(requestSz, ocsp->cm->heap, DYNAMIC_TYPE_OCSP);
    if (request == NULL) {
        WOLFSSL_LEAVE("CheckCertOCSP", MEMORY_ERROR);
        return MEMORY_ERROR;
    }

#ifdef WOLFSSL_SMALL_STACK
    newStatus = (CertStatus*)XMALLOC(sizeof(CertStatus), NULL,
                                                       DYNAMIC_TYPE_TMP_BUFFER);
    ocspResponse = (OcspResponse*)XMALLOC(sizeof(OcspResponse), NULL,
                                                       DYNAMIC_TYPE_TMP_BUFFER);

    if (newStatus == NULL || ocspResponse == NULL) {
        if (newStatus)    XFREE(newStatus,    NULL, DYNAMIC_TYPE_TMP_BUFFER);
        if (ocspResponse) XFREE(ocspResponse, NULL, DYNAMIC_TYPE_TMP_BUFFER);

        XFREE(request, NULL, DYNAMIC_TYPE_OCSP);

        WOLFSSL_LEAVE("CheckCertOCSP", MEMORY_ERROR);
        return MEMORY_E;
    }
#endif

    requestSz = EncodeOcspRequest(ocspRequest, request, requestSz);

    if (ocsp->cm->ocspIOCb)
        ret = ocsp->cm->ocspIOCb(ocsp->cm->ocspIOCtx, url, urlSz,
                                                 request, requestSz, &response);

    if (ret >= 0 && response) {
        XMEMSET(newStatus, 0, sizeof(CertStatus));

        InitOcspResponse(ocspResponse, newStatus, response, ret);
        OcspResponseDecode(ocspResponse, ocsp->cm, ocsp->cm->heap);

        if (ocspResponse->responseStatus != OCSP_SUCCESSFUL)
            ret = OCSP_LOOKUP_FAIL;
        else {
            if (CompareOcspReqResp(ocspRequest, ocspResponse) == 0) {
                if (responseBuffer) {
                    responseBuffer->buffer = (byte*)XMALLOC(ret, ocsp->cm->heap,
                                                       DYNAMIC_TYPE_TMP_BUFFER);

                    if (responseBuffer->buffer) {
                        responseBuffer->length = ret;
                        XMEMCPY(responseBuffer->buffer, response, ret);
                    }
                }

                ret = xstat2err(ocspResponse->status->status);

                if (LockMutex(&ocsp->ocspLock) != 0)
                    ret = BAD_MUTEX_E;
                else {
                    if (status != NULL) {
                        if (status->rawOcspResponse)
                            XFREE(status->rawOcspResponse, ocsp->cm->heap,
                                                      DYNAMIC_TYPE_OCSP_STATUS);

                        /* Replace existing certificate entry with updated */
                        XMEMCPY(status, newStatus, sizeof(CertStatus));
                    }
                    else {
                        /* Save new certificate entry */
                        status = (CertStatus*)XMALLOC(sizeof(CertStatus),
                                      ocsp->cm->heap, DYNAMIC_TYPE_OCSP_STATUS);
                        if (status != NULL) {
                            XMEMCPY(status, newStatus, sizeof(CertStatus));
                            status->next  = entry->status;
                            entry->status = status;
                            entry->totalStatus++;
                        }
                    }

                    if (status && responseBuffer && responseBuffer->buffer) {
                        status->rawOcspResponse = (byte*)XMALLOC(
                                                   responseBuffer->length,
                                                   ocsp->cm->heap,
                                                   DYNAMIC_TYPE_OCSP_STATUS);

                        if (status->rawOcspResponse) {
                            status->rawOcspResponseSz = responseBuffer->length;
                            XMEMCPY(status->rawOcspResponse,
                                    responseBuffer->buffer,
                                    responseBuffer->length);
                        }
                    }

                    UnLockMutex(&ocsp->ocspLock);
                }
            }
            else
                ret = OCSP_LOOKUP_FAIL;
        }
    }
    else
        ret = OCSP_LOOKUP_FAIL;

#ifdef WOLFSSL_SMALL_STACK
    XFREE(newStatus,    NULL, DYNAMIC_TYPE_TMP_BUFFER);
    XFREE(ocspResponse, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (response != NULL && ocsp->cm->ocspRespFreeCb)
        ocsp->cm->ocspRespFreeCb(ocsp->cm->ocspIOCtx, response);

    WOLFSSL_LEAVE("CheckOcspRequest", ret);
    return ret;
}


#else /* HAVE_OCSP */


#ifdef _MSC_VER
    /* 4206 warning for blank file */
    #pragma warning(disable: 4206)
#endif


#endif /* HAVE_OCSP */
#endif /* WOLFCRYPT_ONLY */

