/* errno.h - errno numbers */

/*
 * Copyright 2021, 2024 NXP
 * Copyright (c) 1984-1999, 2012 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Copyright (c) 1982, 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)errno.h	7.1 (Berkeley) 6/4/86
 */
 
/*  Please Note: Source file include the errno.h should be placed after other inlcuded errno.h files to
 *  avoid the multiple definitions of error code numbers.
 */
#ifndef _BLUETOOTH_ERRCODE_H_
#define _BLUETOOTH_ERRCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef EPERM
#define EPERM 1         /**< Not owner */
#endif 
#ifndef ENOENT
#define ENOENT 2        /**< No such file or directory */
#endif 
#ifndef ESRCH
#define ESRCH 3         /**< No such context */
#endif 
#ifndef EINTR
#define EINTR 4         /**< Interrupted system call */
#endif 
#ifndef EIO
#define EIO 5           /**< I/O error */
#endif 
#ifndef ENXIO
#define ENXIO 6         /**< No such device or address */
#endif 
#ifndef E2BIG
#define E2BIG 7         /**< Arg list too long */
#endif 
#ifndef ENOEXEC
#define ENOEXEC 8       /**< Exec format error */
#endif 
#ifndef EBADF
#define EBADF 9         /**< Bad file number */
#endif 
#ifndef ECHILD
#define ECHILD 10       /**< No children */
#endif 
#ifndef EAGAIN
#define EAGAIN 11       /**< No more contexts */
#endif 
#ifndef ENOMEM
#define ENOMEM 12       /**< Not enough core */
#endif 
#ifndef EACCES
#define EACCES 13       /**< Permission denied */
#endif 
#ifndef EFAULT
#define EFAULT 14       /**< Bad address */
#endif 
#ifndef ENOTBLK
#define ENOTBLK 15      /**< Block device required */
#endif 
#ifndef EBUSY
#define EBUSY 16        /**< Mount device busy */
#endif 
#ifndef EEXIST
#define EEXIST 17       /**< File exists */
#endif 
#ifndef EXDEV
#define EXDEV 18        /**< Cross-device link */
#endif 
#ifndef ENODEV
#define ENODEV 19       /**< No such device */
#endif 
#ifndef ENOTDIR
#define ENOTDIR 20      /**< Not a directory */
#endif 
#ifndef EISDIR
#define EISDIR 21       /**< Is a directory */
#endif 
#ifndef EINVAL
#define EINVAL 22       /**< Invalid argument */
#endif 
#ifndef ENFILE
#define ENFILE 23       /**< File table overflow */
#endif 
#ifndef EMFILE
#define EMFILE 24       /**< Too many open files */
#endif 
#ifndef ENOTTY
#define ENOTTY 25       /**< Not a typewriter */
#endif 
#ifndef ETXTBSY
#define ETXTBSY 26      /**< Text file busy */
#endif 
#ifndef EFBIG
#define EFBIG 27        /**< File too large */
#endif 
#ifndef ENOSPC
#define ENOSPC 28       /**< No space left on device */
#endif 
#ifndef ESPIPE
#define ESPIPE 29       /**< Illegal seek */
#endif 
#ifndef EROFS
#define EROFS 30        /**< Read-only file system */
#endif 
#ifndef EMLINK
#define EMLINK 31       /**< Too many links */
#endif 
#ifndef EPIPE
#define EPIPE 32        /**< Broken pipe */
#endif 
#ifndef EDOM
#define EDOM 33         /**< Argument too large */
#endif 
#ifndef ERANGE
#define ERANGE 34       /**< Result too large */
#endif 
#ifndef ENOMSG
#define ENOMSG 35       /**< Unexpected message type */
#endif 
#ifndef EDEADLK
#define EDEADLK 45      /**< Resource deadlock avoided */
#endif 
#ifndef ENOLCK
#define ENOLCK 46       /**< No locks available */
#endif 
#ifndef ENOSTR
#define ENOSTR 60       /**< STREAMS device required */
#endif 
#ifndef ENODATA
#define ENODATA 61      /**< Missing expected message data */
#endif 
#ifndef ETIME
#define ETIME 62        /**< STREAMS timeout occurred */
#endif 
#ifndef ENOSR
#define ENOSR 63        /**< Insufficient memory */
#endif 
#ifndef EPROTO
#define EPROTO 71       /**< Generic STREAMS error */
#endif 
#ifndef EBADMSG
#define EBADMSG 77      /**< Invalid STREAMS message */
#endif 
#ifndef ENOSYS
#define ENOSYS 88       /**< Function not implemented */
#endif 
#ifndef ENOTEMPTY
#define ENOTEMPTY 90    /**< Directory not empty */
#endif 
#ifndef ENAMETOOLONG
#define ENAMETOOLONG 91 /**< File name too long */
#endif 
#ifndef ELOOP
#define ELOOP 92        /**< Too many levels of symbolic links */
#endif 
#ifndef EOPNOTSUPP
#define EOPNOTSUPP 95   /**< Operation not supported on socket */
#endif 
#ifndef EPFNOSUPPORT
#define EPFNOSUPPORT 96 /**< Protocol family not supported */
#endif 
#ifndef ECONNRESET
#define ECONNRESET 104   /**< Connection reset by peer */
#endif 
#ifndef ENOBUFS
#define ENOBUFS 105      /**< No buffer space available */
#endif 
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT 106 /**< Addr family not supported */
#endif 
#ifndef EPROTOTYPE
#define EPROTOTYPE 107   /**< Protocol wrong type for socket */
#endif 
#ifndef ENOTSOCK
#define ENOTSOCK 108     /**< Socket operation on non-socket */
#endif 
#ifndef ENOPROTOOPT
#define ENOPROTOOPT 109  /**< Protocol not available */
#endif 
#ifndef ESHUTDOWN
#define ESHUTDOWN 110    /**< Can't send after socket shutdown */
#endif 
#ifndef ECONNREFUSED
#define ECONNREFUSED 111 /**< Connection refused */
#endif 
#ifndef EADDRINUSE
#define EADDRINUSE 112   /**< Address already in use */
#endif 
#ifndef ECONNABORTED
#define ECONNABORTED 113 /**< Software caused connection abort */
#endif 
#ifndef ENETUNREACH
#define ENETUNREACH 114  /**< Network is unreachable */
#endif 
#ifndef ENETDOWN
#define ENETDOWN 115     /**< Network is down */
#endif 
#ifndef ETIMEDOUT
#define ETIMEDOUT 116    /**< Connection timed out */
#endif 
#ifndef EHOSTDOWN
#define EHOSTDOWN 117    /**< Host is down */
#endif 
#ifndef EHOSTUNREACH
#define EHOSTUNREACH 118 /**< No route to host */
#endif 
#ifndef EINPROGRESS
#define EINPROGRESS 119  /**< Operation now in progress */
#endif 
#ifndef EALREADY
#define EALREADY 120     /**< Operation already in progress */
#endif 
#ifndef EDESTADDRREQ
#define EDESTADDRREQ 121 /**< Destination address required */
#endif 
#ifndef EMSGSIZE
#define EMSGSIZE 122        /**< Message size */
#endif 
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT 123 /**< Protocol not supported */
#endif 
#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT 124 /**< Socket type not supported */
#endif 
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL 125   /**< Can't assign requested address */
#endif 
#ifndef ENETRESET
#define ENETRESET 126       /**< Network dropped connection on reset */
#endif 
#ifndef EISCONN
#define EISCONN 127         /**< Socket is already connected */
#endif 
#ifndef ENOTCONN
#define ENOTCONN 128        /**< Socket is not connected */
#endif 
#ifndef ETOOMANYREFS
#define ETOOMANYREFS 129    /**< Too many references: can't splice */
#endif 
#ifndef ENOTSUP
#define ENOTSUP 134         /**< Unsupported value */
#endif 
#ifndef EILSEQ
#define EILSEQ 138          /**< Illegal byte sequence */
#endif 
#ifndef EOVERFLOW
#define EOVERFLOW 139       /**< Value overflow */
#endif 
#ifndef ECANCELED
#define ECANCELED 140       /**< Operation canceled */
#endif 

#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN /**< Operation would block */
#endif 


#ifndef errno
extern int errno;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BLUETOOTH_ERRCODE_H_ */
