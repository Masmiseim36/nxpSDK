/* crl.c
 *
 * Copyright (C) 2006-2016 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * http://www.wolfssl.com
 */



  /* Name change compatibility layer no longer needs included here */

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#ifndef WOLFCRYPT_ONLY
#ifdef HAVE_CRL

#include <wolfssl/internal.h>
#include <wolfssl/error-ssl.h>

#ifndef NO_FILESYSTEM
    #include <dirent.h>
    #include <sys/stat.h>
#endif

#include <string.h>

#ifdef HAVE_CRL_MONITOR
    static int StopMonitor(int mfd);
#endif


/* Initialize CRL members */
int InitCRL(WOLFSSL_CRL* crl, WOLFSSL_CERT_MANAGER* cm)
{
    WOLFSSL_ENTER("InitCRL");

    crl->heap = cm->heap;
    crl->cm = cm;
    crl->crlList = NULL;
    crl->monitors[0].path = NULL;
    crl->monitors[1].path = NULL;
#ifdef HAVE_CRL_MONITOR
    crl->tid   =  0;
    crl->mfd   = -1;    /* mfd for bsd is kqueue fd, eventfd for linux */
    crl->setup = 0;     /* thread setup done predicate */
    if (pthread_cond_init(&crl->cond, 0) != 0) {
        WOLFSSL_MSG("Pthread condition init failed");
        return BAD_COND_E;
    }
#endif
    if (InitMutex(&crl->crlLock) != 0) {
        WOLFSSL_MSG("Init Mutex failed");
        return BAD_MUTEX_E;
    }

    return 0;
}


/* Initialize CRL Entry */
static int InitCRL_Entry(CRL_Entry* crle, DecodedCRL* dcrl)
{
    WOLFSSL_ENTER("InitCRL_Entry");

    XMEMCPY(crle->issuerHash, dcrl->issuerHash, CRL_DIGEST_SIZE);
    /* XMEMCPY(crle->crlHash, dcrl->crlHash, CRL_DIGEST_SIZE);
     *   copy the hash here if needed for optimized comparisons */
    XMEMCPY(crle->lastDate, dcrl->lastDate, MAX_DATE_SIZE);
    XMEMCPY(crle->nextDate, dcrl->nextDate, MAX_DATE_SIZE);
    crle->lastDateFormat = dcrl->lastDateFormat;
    crle->nextDateFormat = dcrl->nextDateFormat;

    crle->certs = dcrl->certs;   /* take ownsership */
    dcrl->certs = NULL;
    crle->totalCerts = dcrl->totalCerts;

    return 0;
}


/* Free all CRL Entry resources */
static void FreeCRL_Entry(CRL_Entry* crle, void* heap)
{
    RevokedCert* tmp = crle->certs;

    WOLFSSL_ENTER("FreeCRL_Entry");

    while(tmp) {
        RevokedCert* next = tmp->next;
        XFREE(tmp, heap, DYNAMIC_TYPE_REVOKED);
        tmp = next;
    }

    (void)heap;
}



/* Free all CRL resources */
void FreeCRL(WOLFSSL_CRL* crl, int dynamic)
{
    CRL_Entry* tmp = crl->crlList;

    WOLFSSL_ENTER("FreeCRL");

    if (crl->monitors[0].path)
        XFREE(crl->monitors[0].path, crl->heap, DYNAMIC_TYPE_CRL_MONITOR);

    if (crl->monitors[1].path)
        XFREE(crl->monitors[1].path, crl->heap, DYNAMIC_TYPE_CRL_MONITOR);

    while(tmp) {
        CRL_Entry* next = tmp->next;
        FreeCRL_Entry(tmp, crl->heap);
        XFREE(tmp, crl->heap, DYNAMIC_TYPE_CRL_ENTRY);
        tmp = next;
    }

#ifdef HAVE_CRL_MONITOR
    if (crl->tid != 0) {
        WOLFSSL_MSG("stopping monitor thread");
        if (StopMonitor(crl->mfd) == 0)
            pthread_join(crl->tid, NULL);
        else {
            WOLFSSL_MSG("stop monitor failed");
        }
    }
    pthread_cond_destroy(&crl->cond);
#endif
    FreeMutex(&crl->crlLock);
    if (dynamic)   /* free self */
        XFREE(crl, crl->heap, DYNAMIC_TYPE_CRL);
}


/* Is the cert ok with CRL, return 0 on success */
int CheckCertCRL(WOLFSSL_CRL* crl, DecodedCert* cert)
{
    CRL_Entry* crle;
    int        foundEntry = 0;
    int        ret = 0;

    WOLFSSL_ENTER("CheckCertCRL");

    if (LockMutex(&crl->crlLock) != 0) {
        WOLFSSL_MSG("LockMutex failed");
        return BAD_MUTEX_E;
    }

    crle = crl->crlList;

    while (crle) {
        if (XMEMCMP(crle->issuerHash, cert->issuerHash, CRL_DIGEST_SIZE) == 0) {
            int doNextDate = 1;

            WOLFSSL_MSG("Found CRL Entry on list");
            WOLFSSL_MSG("Checking next date validity");

            #ifdef WOLFSSL_NO_CRL_NEXT_DATE
                if (crle->nextDateFormat == ASN_OTHER_TYPE)
                    doNextDate = 0;  /* skip */
            #endif

            if (doNextDate && !ValidateDate(crle->nextDate,
                                            crle->nextDateFormat, AFTER)) {
                WOLFSSL_MSG("CRL next date is no longer valid");
                ret = ASN_AFTER_DATE_E;
            }
            else
                foundEntry = 1;
            break;
        }
        crle = crle->next;
    }

    if (foundEntry) {
        RevokedCert* rc = crle->certs;

        while (rc) {
            if (XMEMCMP(rc->serialNumber, cert->serial, rc->serialSz) == 0) {
                WOLFSSL_MSG("Cert revoked");
                ret = CRL_CERT_REVOKED;
                break;
            }
            rc = rc->next;
        }
    }

    UnLockMutex(&crl->crlLock);

    if (foundEntry == 0) {
        WOLFSSL_MSG("Couldn't find CRL for status check");
        ret = CRL_MISSING;
        if (crl->cm->cbMissingCRL) {
            char url[256];

            WOLFSSL_MSG("Issuing missing CRL callback");
            url[0] = '\0';
            if (cert->extCrlInfoSz < (int)sizeof(url) -1 ) {
                XMEMCPY(url, cert->extCrlInfo, cert->extCrlInfoSz);
                url[cert->extCrlInfoSz] = '\0';
            }
            else  {
                WOLFSSL_MSG("CRL url too long");
            }
            crl->cm->cbMissingCRL(url);
        }
    }


    return ret;
}


/* Add Decoded CRL, 0 on success */
static int AddCRL(WOLFSSL_CRL* crl, DecodedCRL* dcrl)
{
    CRL_Entry* crle;

    WOLFSSL_ENTER("AddCRL");

    crle = (CRL_Entry*)XMALLOC(sizeof(CRL_Entry), crl->heap, DYNAMIC_TYPE_CRL_ENTRY);
    if (crle == NULL) {
        WOLFSSL_MSG("alloc CRL Entry failed");
        return -1;
    }

    if (InitCRL_Entry(crle, dcrl) < 0) {
        WOLFSSL_MSG("Init CRL Entry failed");
        XFREE(crle, crl->heap, DYNAMIC_TYPE_CRL_ENTRY);
        return -1;
    }

    if (LockMutex(&crl->crlLock) != 0) {
        WOLFSSL_MSG("LockMutex failed");
        FreeCRL_Entry(crle, crl->heap);
        XFREE(crle, crl->heap, DYNAMIC_TYPE_CRL_ENTRY);
        return BAD_MUTEX_E;
    }
    crle->next = crl->crlList;
    crl->crlList = crle;
    UnLockMutex(&crl->crlLock);

    return 0;
}


/* Load CRL File of type, SSL_SUCCESS on ok */
int BufferLoadCRL(WOLFSSL_CRL* crl, const byte* buff, long sz, int type)
{
    int          ret = SSL_SUCCESS;
    const byte*  myBuffer = buff;    /* if DER ok, otherwise switch */
    DerBuffer*   der = NULL;
#ifdef WOLFSSL_SMALL_STACK
    DecodedCRL*  dcrl;
#else
    DecodedCRL   dcrl[1];
#endif

    WOLFSSL_ENTER("BufferLoadCRL");

    if (crl == NULL || buff == NULL || sz == 0)
        return BAD_FUNC_ARG;

    if (type == SSL_FILETYPE_PEM) {
        int eccKey = 0;   /* not used */
        EncryptedInfo info;
        info.ctx = NULL;

        ret = PemToDer(buff, sz, CRL_TYPE, &der, NULL, &info, &eccKey);
        if (ret == 0) {
            myBuffer = der->buffer;
            sz = der->length;
        }
        else {
            WOLFSSL_MSG("Pem to Der failed");
            FreeDer(&der);
            return -1;
        }
    }

#ifdef WOLFSSL_SMALL_STACK
    dcrl = (DecodedCRL*)XMALLOC(sizeof(DecodedCRL), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (dcrl == NULL) {
        FreeDer(&der);
        return MEMORY_E;
    }
#endif

    InitDecodedCRL(dcrl, crl->heap);
    ret = ParseCRL(dcrl, myBuffer, (word32)sz, crl->cm);
    if (ret != 0) {
        WOLFSSL_MSG("ParseCRL error");
    }
    else {
        ret = AddCRL(crl, dcrl);
        if (ret != 0) {
            WOLFSSL_MSG("AddCRL error");
        }
    }

    FreeDecodedCRL(dcrl);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(dcrl, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    FreeDer(&der);

    return ret ? ret : SSL_SUCCESS; /* convert 0 to SSL_SUCCESS */
}


#ifdef HAVE_CRL_MONITOR


/* Signal Monitor thread is setup, save status to setup flag, 0 on success */
static int SignalSetup(WOLFSSL_CRL* crl, int status)
{
    int ret;

    /* signal to calling thread we're setup */
    if (LockMutex(&crl->crlLock) != 0) {
        WOLFSSL_MSG("LockMutex crlLock failed");
        return BAD_MUTEX_E;
    }

        crl->setup = status;
        ret = pthread_cond_signal(&crl->cond);

    UnLockMutex(&crl->crlLock);

    if (ret != 0)
        return BAD_COND_E;

    return 0;
}


/* read in new CRL entries and save new list */
static int SwapLists(WOLFSSL_CRL* crl)
{
    int        ret;
    CRL_Entry* newList;
#ifdef WOLFSSL_SMALL_STACK
    WOLFSSL_CRL* tmp;
#else
    WOLFSSL_CRL tmp[1];
#endif

#ifdef WOLFSSL_SMALL_STACK
    tmp = (WOLFSSL_CRL*)XMALLOC(sizeof(WOLFSSL_CRL), NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (tmp == NULL)
        return MEMORY_E;
#endif

    if (InitCRL(tmp, crl->cm) < 0) {
        WOLFSSL_MSG("Init tmp CRL failed");
#ifdef WOLFSSL_SMALL_STACK
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return -1;
    }

    if (crl->monitors[0].path) {
        ret = LoadCRL(tmp, crl->monitors[0].path, SSL_FILETYPE_PEM, 0);
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("PEM LoadCRL on dir change failed");
            FreeCRL(tmp, 0);
#ifdef WOLFSSL_SMALL_STACK
            XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
            return -1;
        }
    }

    if (crl->monitors[1].path) {
        ret = LoadCRL(tmp, crl->monitors[1].path, SSL_FILETYPE_ASN1, 0);
        if (ret != SSL_SUCCESS) {
            WOLFSSL_MSG("DER LoadCRL on dir change failed");
            FreeCRL(tmp, 0);
#ifdef WOLFSSL_SMALL_STACK
            XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
            return -1;
        }
    }

    if (LockMutex(&crl->crlLock) != 0) {
        WOLFSSL_MSG("LockMutex failed");
        FreeCRL(tmp, 0);
#ifdef WOLFSSL_SMALL_STACK
        XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif
        return -1;
    }

    newList = tmp->crlList;

    /* swap lists */
    tmp->crlList  = crl->crlList;
    crl->crlList = newList;

    UnLockMutex(&crl->crlLock);

    FreeCRL(tmp, 0);

#ifdef WOLFSSL_SMALL_STACK
    XFREE(tmp, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    return 0;
}


#if (defined(__MACH__) || defined(__FreeBSD__))

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __MACH__
    #define XEVENT_MODE O_EVTONLY
#elif defined(__FreeBSD__)
    #define XEVENT_MODE EVFILT_VNODE
#endif


/* we need a unique kqueue user filter fd for crl in case user is doing custom
 * events too */
#ifndef CRL_CUSTOM_FD
    #define CRL_CUSTOM_FD 123456
#endif


/* shutdown monitor thread, 0 on success */
static int StopMonitor(int mfd)
{
    struct kevent change;

    /* trigger custom shutdown */
    EV_SET(&change, CRL_CUSTOM_FD, EVFILT_USER, 0, NOTE_TRIGGER, 0, NULL);
    if (kevent(mfd, &change, 1, NULL, 0, NULL) < 0) {
        WOLFSSL_MSG("kevent trigger customer event failed");
        return -1;
    }

    return 0;
}


/* OS X  monitoring */
static void* DoMonitor(void* arg)
{
    int fPEM, fDER;
    struct kevent change;

    WOLFSSL_CRL* crl = (WOLFSSL_CRL*)arg;

    WOLFSSL_ENTER("DoMonitor");

    crl->mfd = kqueue();
    if (crl->mfd == -1) {
        WOLFSSL_MSG("kqueue failed");
        SignalSetup(crl, MONITOR_SETUP_E);
        return NULL;
    }

    /* listen for custom shutdown event */
    EV_SET(&change, CRL_CUSTOM_FD, EVFILT_USER, EV_ADD, 0, 0, NULL);
    if (kevent(crl->mfd, &change, 1, NULL, 0, NULL) < 0) {
        WOLFSSL_MSG("kevent monitor customer event failed");
        SignalSetup(crl, MONITOR_SETUP_E);
        close(crl->mfd);
        return NULL;
    }

    fPEM = -1;
    fDER = -1;

    if (crl->monitors[0].path) {
        fPEM = open(crl->monitors[0].path, XEVENT_MODE);
        if (fPEM == -1) {
            WOLFSSL_MSG("PEM event dir open failed");
            SignalSetup(crl, MONITOR_SETUP_E);
            close(crl->mfd);
            return NULL;
        }
    }

    if (crl->monitors[1].path) {
        fDER = open(crl->monitors[1].path, XEVENT_MODE);
        if (fDER == -1) {
            WOLFSSL_MSG("DER event dir open failed");
            if (fPEM != -1)
                close(fPEM);
            close(crl->mfd);
            SignalSetup(crl, MONITOR_SETUP_E);
            return NULL;
        }
    }

    if (fPEM != -1)
        EV_SET(&change, fPEM, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_ONESHOT,
                NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB, 0, 0);

    if (fDER != -1)
        EV_SET(&change, fDER, EVFILT_VNODE, EV_ADD | EV_ENABLE | EV_ONESHOT,
                NOTE_DELETE | NOTE_EXTEND | NOTE_WRITE | NOTE_ATTRIB, 0, 0);

    /* signal to calling thread we're setup */
    if (SignalSetup(crl, 1) != 0) {
        if (fPEM != -1)
            close(fPEM);
        if (fDER != -1)
            close(fDER);
        close(crl->mfd);
        return NULL;
    }

    for (;;) {
        struct kevent event;
        int           numEvents = kevent(crl->mfd, &change, 1, &event, 1, NULL);

        WOLFSSL_MSG("Got kevent");

        if (numEvents == -1) {
            WOLFSSL_MSG("kevent problem, continue");
            continue;
        }

        if (event.filter == EVFILT_USER) {
            WOLFSSL_MSG("Got user shutdown event, breaking out");
            break;
        }

        if (SwapLists(crl) < 0) {
            WOLFSSL_MSG("SwapLists problem, continue");
        }
    }

    if (fPEM != -1)
        close(fPEM);
    if (fDER != -1)
        close(fDER);

    close(crl->mfd);

    return NULL;
}


#elif defined(__linux__)

#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/eventfd.h>
#include <unistd.h>


#ifndef max
    static INLINE int max(int a, int b)
    {
        return a > b ? a : b;
    }
#endif /* max */


/* shutdown monitor thread, 0 on success */
static int StopMonitor(int mfd)
{
    word64 w64 = 1;

    /* write to our custom event */
    if (write(mfd, &w64, sizeof(w64)) < 0) {
        WOLFSSL_MSG("StopMonitor write failed");
        return -1;
    }

    return 0;
}


/* linux monitoring */
static void* DoMonitor(void* arg)
{
    int         notifyFd;
    int         wd  = -1;
    WOLFSSL_CRL* crl = (WOLFSSL_CRL*)arg;
#ifdef WOLFSSL_SMALL_STACK
    char*       buff;
#else
    char        buff[8192];
#endif

    WOLFSSL_ENTER("DoMonitor");

    crl->mfd = eventfd(0, 0);  /* our custom shutdown event */
    if (crl->mfd < 0) {
        WOLFSSL_MSG("eventfd failed");
        SignalSetup(crl, MONITOR_SETUP_E);
        return NULL;
    }

    notifyFd = inotify_init();
    if (notifyFd < 0) {
        WOLFSSL_MSG("inotify failed");
        close(crl->mfd);
        SignalSetup(crl, MONITOR_SETUP_E);
        return NULL;
    }

    if (crl->monitors[0].path) {
        wd = inotify_add_watch(notifyFd, crl->monitors[0].path, IN_CLOSE_WRITE |
                                                                IN_DELETE);
        if (wd < 0) {
            WOLFSSL_MSG("PEM notify add watch failed");
            close(crl->mfd);
            close(notifyFd);
            SignalSetup(crl, MONITOR_SETUP_E);
            return NULL;
        }
    }

    if (crl->monitors[1].path) {
        wd = inotify_add_watch(notifyFd, crl->monitors[1].path, IN_CLOSE_WRITE |
                                                                IN_DELETE);
        if (wd < 0) {
            WOLFSSL_MSG("DER notify add watch failed");
            close(crl->mfd);
            close(notifyFd);
            SignalSetup(crl, MONITOR_SETUP_E);
            return NULL;
        }
    }

#ifdef WOLFSSL_SMALL_STACK
    buff = (char*)XMALLOC(8192, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (buff == NULL)
        return NULL;
#endif

    /* signal to calling thread we're setup */
    if (SignalSetup(crl, 1) != 0) {
        #ifdef WOLFSSL_SMALL_STACK
            XFREE(buff, NULL, DYNAMIC_TYPE_TMP_BUFFER);
        #endif

        if (wd > 0)
            inotify_rm_watch(notifyFd, wd);
        close(crl->mfd);
        close(notifyFd);
        return NULL;
    }

    for (;;) {
        fd_set readfds;
        int    result;
        int    length;

        FD_ZERO(&readfds);
        FD_SET(notifyFd, &readfds);
        FD_SET(crl->mfd, &readfds);

        result = select(max(notifyFd, crl->mfd) + 1, &readfds, NULL, NULL,NULL);

        WOLFSSL_MSG("Got notify event");

        if (result < 0) {
            WOLFSSL_MSG("select problem, continue");
            continue;
        }

        if (FD_ISSET(crl->mfd, &readfds)) {
            WOLFSSL_MSG("got custom shutdown event, breaking out");
            break;
        }

        length = (int) read(notifyFd, buff, 8192);
        if (length < 0) {
            WOLFSSL_MSG("notify read problem, continue");
            continue;
        }

        if (SwapLists(crl) < 0) {
            WOLFSSL_MSG("SwapLists problem, continue");
        }
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(buff, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (wd > 0)
        inotify_rm_watch(notifyFd, wd);
    close(crl->mfd);
    close(notifyFd);

    return NULL;
}


#else

#error "CRL monitor only currently supported on linux or mach"

#endif /* MACH or linux */


/* Start Monitoring the CRL path(s) in a thread */
static int StartMonitorCRL(WOLFSSL_CRL* crl)
{
    int ret = SSL_SUCCESS;

    WOLFSSL_ENTER("StartMonitorCRL");

    if (crl == NULL)
        return BAD_FUNC_ARG;

    if (crl->tid != 0) {
        WOLFSSL_MSG("Monitor thread already running");
        return ret;  /* that's ok, someone already started */
    }

    if (pthread_create(&crl->tid, NULL, DoMonitor, crl) != 0) {
        WOLFSSL_MSG("Thread creation error");
        return THREAD_CREATE_E;
    }

    /* wait for setup to complete */
    if (LockMutex(&crl->crlLock) != 0) {
        WOLFSSL_MSG("LockMutex crlLock error");
        return BAD_MUTEX_E;
    }

        while (crl->setup == 0) {
            if (pthread_cond_wait(&crl->cond, &crl->crlLock) != 0) {
                ret = BAD_COND_E;
                break;
            }
        }

        if (crl->setup < 0)
            ret = crl->setup;  /* store setup error */

    UnLockMutex(&crl->crlLock);

    if (ret < 0) {
        WOLFSSL_MSG("DoMonitor setup failure");
        crl->tid = 0;  /* thread already done */
    }

    return ret;
}


#else /* HAVE_CRL_MONITOR */

#ifndef NO_FILESYSTEM

static int StartMonitorCRL(WOLFSSL_CRL* crl)
{
    (void)crl;

    WOLFSSL_ENTER("StartMonitorCRL");
    WOLFSSL_MSG("Not compiled in");

    return NOT_COMPILED_IN;
}

#endif /* NO_FILESYSTEM */

#endif  /* HAVE_CRL_MONITOR */

#ifndef NO_FILESYSTEM

/* Load CRL path files of type, SSL_SUCCESS on ok */
int LoadCRL(WOLFSSL_CRL* crl, const char* path, int type, int monitor)
{
    struct dirent* entry;
    DIR*           dir;
    int            ret = SSL_SUCCESS;
#ifdef WOLFSSL_SMALL_STACK
    char*          name;
#else
    char           name[MAX_FILENAME_SZ];
#endif

    WOLFSSL_ENTER("LoadCRL");
    if (crl == NULL)
        return BAD_FUNC_ARG;

    dir = opendir(path);
    if (dir == NULL) {
        WOLFSSL_MSG("opendir path crl load failed");
        return BAD_PATH_ERROR;
    }

#ifdef WOLFSSL_SMALL_STACK
    name = (char*)XMALLOC(MAX_FILENAME_SZ, NULL, DYNAMIC_TYPE_TMP_BUFFER);
    if (name == NULL)
        return MEMORY_E;
#endif

    while ( (entry = readdir(dir)) != NULL) {
        struct stat s;

        XMEMSET(name, 0, MAX_FILENAME_SZ);
        XSTRNCPY(name, path, MAX_FILENAME_SZ/2 - 2);
        XSTRNCAT(name, "/", 1);
        XSTRNCAT(name, entry->d_name, MAX_FILENAME_SZ/2);

        if (stat(name, &s) != 0) {
            WOLFSSL_MSG("stat on name failed");
            continue;
        }
        if (s.st_mode & S_IFREG) {

            if (type == SSL_FILETYPE_PEM) {
                if (XSTRSTR(entry->d_name, ".pem") == NULL) {
                    WOLFSSL_MSG("not .pem file, skipping");
                    continue;
                }
            }
            else {
                if (XSTRSTR(entry->d_name, ".der") == NULL &&
                    XSTRSTR(entry->d_name, ".crl") == NULL) {

                    WOLFSSL_MSG("not .der or .crl file, skipping");
                    continue;
                }
            }

            if (ProcessFile(NULL, name, type, CRL_TYPE, NULL, 0, crl)
                                                               != SSL_SUCCESS) {
                WOLFSSL_MSG("CRL file load failed, continuing");
            }
        }
    }

#ifdef WOLFSSL_SMALL_STACK
    XFREE(name, NULL, DYNAMIC_TYPE_TMP_BUFFER);
#endif

    if (monitor & WOLFSSL_CRL_MONITOR) {
        word32 pathLen;
        char* pathBuf;

        WOLFSSL_MSG("monitor path requested");

        pathLen = (word32)XSTRLEN(path);
        pathBuf = (char*)XMALLOC(pathLen+1, crl->heap,DYNAMIC_TYPE_CRL_MONITOR);
        if (pathBuf) {
            XSTRNCPY(pathBuf, path, pathLen);
            pathBuf[pathLen] = '\0'; /* Null Terminate */

            if (type == SSL_FILETYPE_PEM) {
                crl->monitors[0].path = pathBuf;
                crl->monitors[0].type = SSL_FILETYPE_PEM;
            } else {
                crl->monitors[1].path = pathBuf;
                crl->monitors[1].type = SSL_FILETYPE_ASN1;
            }

            if (monitor & WOLFSSL_CRL_START_MON) {
                WOLFSSL_MSG("start monitoring requested");

                ret = StartMonitorCRL(crl);
            }
        }
        else {
            ret = MEMORY_E;
        }
    }

    closedir(dir);

    return ret;
}

#endif /* NO_FILESYSTEM */

#endif /* HAVE_CRL */
#endif /* !WOLFCRYPT_ONLY */
