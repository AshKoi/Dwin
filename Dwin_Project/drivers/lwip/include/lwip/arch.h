/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __LWIP_ARCH_H__
#define __LWIP_ARCH_H__

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1234
#endif

#ifndef BIG_ENDIAN
#define BIG_ENDIAN 4321
#endif

#include "lwip/include/arch/cc.h"

/** Temporary: define format string for size_t if not defined in cc.h */
#ifndef SZT_F
#define SZT_F U32_F
#endif /* SZT_F */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PACK_STRUCT_BEGIN
#define PACK_STRUCT_BEGIN    __packed
#endif /* PACK_STRUCT_BEGIN */

#ifndef PACK_STRUCT_END
#define PACK_STRUCT_END
#endif /* PACK_STRUCT_END */

#ifndef PACK_STRUCT_FIELD
#define PACK_STRUCT_FIELD(x)    __packed x
#endif /* PACK_STRUCT_FIELD */


#ifndef LWIP_UNUSED_ARG
#define LWIP_UNUSED_ARG(x) (void)x
#endif /* LWIP_UNUSED_ARG */ 


#ifdef LWIP_PROVIDE_ERRNO

#define  LWIP_ERRNO_EPERM          1  /* Operation not permitted */
#define  LWIP_ERRNO_ENOENT         2  /* No such file or directory */
#define  LWIP_ERRNO_ESRCH          3  /* No such process */
#define  LWIP_ERRNO_EINTR          4  /* Interrupted system call */
#define  LWIP_ERRNO_EIO            5  /* I/O error */
#define  LWIP_ERRNO_ENXIO          6  /* No such device or address */
#define  LWIP_ERRNO_E2BIG          7  /* Arg list too long */
#define  LWIP_ERRNO_ENOEXEC        8  /* Exec format error */
#define  LWIP_ERRNO_EBADF          9  /* Bad file number */
#define  LWIP_ERRNO_ECHILD        10  /* No child processes */
#define  LWIP_ERRNO_EAGAIN        11  /* Try again */
#define  LWIP_ERRNO_ENOMEM        12  /* Out of memory */
#define  LWIP_ERRNO_EACCES        13  /* Permission denied */
#define  LWIP_ERRNO_EFAULT        14  /* Bad address */
#define  LWIP_ERRNO_ENOTBLK       15  /* Block device required */
#define  LWIP_ERRNO_EBUSY         16  /* Device or resource busy */
#define  LWIP_ERRNO_EEXIST        17  /* File exists */
#define  LWIP_ERRNO_EXDEV         18  /* Cross-device link */
#define  LWIP_ERRNO_ENODEV        19  /* No such device */
#define  LWIP_ERRNO_ENOTDIR       20  /* Not a directory */
#define  LWIP_ERRNO_EISDIR        21  /* Is a directory */
#define  LWIP_ERRNO_EINVAL        22  /* Invalid argument */
#define  LWIP_ERRNO_ENFILE        23  /* File table overflow */
#define  LWIP_ERRNO_EMFILE        24  /* Too many open files */
#define  LWIP_ERRNO_ENOTTY        25  /* Not a typewriter */
#define  LWIP_ERRNO_ETXTBSY       26  /* Text file busy */
#define  LWIP_ERRNO_EFBIG         27  /* File too large */
#define  LWIP_ERRNO_ENOSPC        28  /* No space left on device */
#define  LWIP_ERRNO_ESPIPE        29  /* Illegal seek */
#define  LWIP_ERRNO_EROFS         30  /* Read-only file system */
#define  LWIP_ERRNO_EMLINK        31  /* Too many links */
#define  LWIP_ERRNO_EPIPE         32  /* Broken pipe */
#define  LWIP_ERRNO_EDOM          33  /* Math argument out of domain of func */
#define  LWIP_ERRNO_ERANGE        34  /* Math result not representable */
#define  LWIP_ERRNO_EDEADLK       35  /* Resource deadlock would occur */
#define  LWIP_ERRNO_ENAMETOOLONG  36  /* File name too long */
#define  LWIP_ERRNO_ENOLCK        37  /* No record locks available */
#define  LWIP_ERRNO_ENOSYS        38  /* Function not implemented */
#define  LWIP_ERRNO_ENOTEMPTY     39  /* Directory not empty */
#define  LWIP_ERRNO_ELOOP         40  /* Too many symbolic links encountered */
#define  LWIP_ERRNO_EWOULDBLOCK   LWIP_ERRNO_EAGAIN  /* Operation would block */
#define  LWIP_ERRNO_ENOMSG        42  /* No message of desired type */
#define  LWIP_ERRNO_EIDRM         43  /* Identifier removed */
#define  LWIP_ERRNO_ECHRNG        44  /* Channel number out of range */
#define  LWIP_ERRNO_EL2NSYNC      45  /* Level 2 not synchronized */
#define  LWIP_ERRNO_EL3HLT        46  /* Level 3 halted */
#define  LWIP_ERRNO_EL3RST        47  /* Level 3 reset */
#define  LWIP_ERRNO_ELNRNG        48  /* Link number out of range */
#define  LWIP_ERRNO_EUNATCH       49  /* Protocol driver not attached */
#define  LWIP_ERRNO_ENOCSI        50  /* No CSI structure available */
#define  LWIP_ERRNO_EL2HLT        51  /* Level 2 halted */
#define  LWIP_ERRNO_EBADE         52  /* Invalid exchange */
#define  LWIP_ERRNO_EBADR         53  /* Invalid request descriptor */
#define  LWIP_ERRNO_EXFULL        54  /* Exchange full */
#define  LWIP_ERRNO_ENOANO        55  /* No anode */
#define  LWIP_ERRNO_EBADRQC       56  /* Invalid request code */
#define  LWIP_ERRNO_EBADSLT       57  /* Invalid slot */

#define  LWIP_ERRNO_EDEADLOCK     LWIP_ERRNO_EDEADLK

#define  LWIP_ERRNO_EBFONT        59  /* Bad font file format */
#define  LWIP_ERRNO_ENOSTR        60  /* Device not a stream */
#define  LWIP_ERRNO_ENODATA       61  /* No data available */
#define  LWIP_ERRNO_ETIME         62  /* Timer expired */
#define  LWIP_ERRNO_ENOSR         63  /* Out of streams resources */
#define  LWIP_ERRNO_ENONET        64  /* Machine is not on the network */
#define  LWIP_ERRNO_ENOPKG        65  /* Package not installed */
#define  LWIP_ERRNO_EREMOTE       66  /* Object is remote */
#define  LWIP_ERRNO_ENOLINK       67  /* Link has been severed */
#define  LWIP_ERRNO_EADV          68  /* Advertise error */
#define  LWIP_ERRNO_ESRMNT        69  /* Srmount error */
#define  LWIP_ERRNO_ECOMM         70  /* Communication error on send */
#define  LWIP_ERRNO_EPROTO        71  /* Protocol error */
#define  LWIP_ERRNO_EMULTIHOP     72  /* Multihop attempted */
#define  LWIP_ERRNO_EDOTDOT       73  /* RFS specific error */
#define  LWIP_ERRNO_EBADMSG       74  /* Not a data message */
#define  LWIP_ERRNO_EOVERFLOW     75  /* Value too large for defined data type */
#define  LWIP_ERRNO_ENOTUNIQ      76  /* Name not unique on network */
#define  LWIP_ERRNO_EBADFD        77  /* File descriptor in bad state */
#define  LWIP_ERRNO_EREMCHG       78  /* Remote address changed */
#define  LWIP_ERRNO_ELIBACC       79  /* Can not access a needed shared library */
#define  LWIP_ERRNO_ELIBBAD       80  /* Accessing a corrupted shared library */
#define  LWIP_ERRNO_ELIBSCN       81  /* .lib section in a.out corrupted */
#define  LWIP_ERRNO_ELIBMAX       82  /* Attempting to link in too many shared libraries */
#define  LWIP_ERRNO_ELIBEXEC      83  /* Cannot exec a shared library directly */
#define  LWIP_ERRNO_EILSEQ        84  /* Illegal byte sequence */
#define  LWIP_ERRNO_ERESTART      85  /* Interrupted system call should be restarted */
#define  LWIP_ERRNO_ESTRPIPE      86  /* Streams pipe error */
#define  LWIP_ERRNO_EUSERS        87  /* Too many users */
#define  LWIP_ERRNO_ENOTSOCK      88  /* Socket operation on non-socket */
#define  LWIP_ERRNO_EDESTADDRREQ  89  /* Destination address required */
#define  LWIP_ERRNO_EMSGSIZE      90  /* Message too long */
#define  LWIP_ERRNO_EPROTOTYPE    91  /* Protocol wrong type for socket */
#define  LWIP_ERRNO_ENOPROTOOPT   92  /* Protocol not available */
#define  LWIP_ERRNO_EPROTONOSUPPORT  93  /* Protocol not supported */
#define  LWIP_ERRNO_ESOCKTNOSUPPORT  94  /* Socket type not supported */
#define  LWIP_ERRNO_EOPNOTSUPP     95  /* Operation not supported on transport endpoint */
#define  LWIP_ERRNO_EPFNOSUPPORT   96  /* Protocol family not supported */
#define  LWIP_ERRNO_EAFNOSUPPORT   97  /* Address family not supported by protocol */
#define  LWIP_ERRNO_EADDRINUSE     98  /* Address already in use */
#define  LWIP_ERRNO_EADDRNOTAVAIL  99  /* Cannot assign requested address */
#define  LWIP_ERRNO_ENETDOWN      100  /* Network is down */
#define  LWIP_ERRNO_ENETUNREACH   101  /* Network is unreachable */
#define  LWIP_ERRNO_ENETRESET     102  /* Network dropped connection because of reset */
#define  LWIP_ERRNO_ECONNABORTED  103  /* Software caused connection abort */
#define  LWIP_ERRNO_ECONNRESET    104  /* Connection reset by peer */
#define  LWIP_ERRNO_ENOBUFS       105  /* No buffer space available */
#define  LWIP_ERRNO_EISCONN       106  /* Transport endpoint is already connected */
#define  LWIP_ERRNO_ENOTCONN      107  /* Transport endpoint is not connected */
#define  LWIP_ERRNO_ESHUTDOWN     108  /* Cannot send after transport endpoint shutdown */
#define  LWIP_ERRNO_ETOOMANYREFS  109  /* Too many references: cannot splice */
#define  LWIP_ERRNO_ETIMEDOUT     110  /* Connection timed out */
#define  LWIP_ERRNO_ECONNREFUSED  111  /* Connection refused */
#define  LWIP_ERRNO_EHOSTDOWN     112  /* Host is down */
#define  LWIP_ERRNO_EHOSTUNREACH  113  /* No route to host */
#define  LWIP_ERRNO_EALREADY      114  /* Operation already in progress */
#define  LWIP_ERRNO_EINPROGRESS   115  /* Operation now in progress */
#define  LWIP_ERRNO_ESTALE        116  /* Stale NFS file handle */
#define  LWIP_ERRNO_EUCLEAN       117  /* Structure needs cleaning */
#define  LWIP_ERRNO_ENOTNAM       118  /* Not a XENIX named type file */
#define  LWIP_ERRNO_ENAVAIL       119  /* No XENIX semaphores available */
#define  LWIP_ERRNO_EISNAM        120  /* Is a named type file */
#define  LWIP_ERRNO_EREMOTEIO     121  /* Remote I/O error */
#define  LWIP_ERRNO_EDQUOT        122  /* Quota exceeded */

#define  LWIP_ERRNO_ENOMEDIUM     123  /* No medium found */
#define  LWIP_ERRNO_EMEDIUMTYPE   124  /* Wrong medium type */


#define LWIP_ERRNO_ENSROK                    0 /* DNS server returned answer with no data */
#define LWIP_ERRNO_ENSRNODATA              160 /* DNS server returned answer with no data */
#define LWIP_ERRNO_ENSRFORMERR             161 /* DNS server claims query was misformatted */
#define LWIP_ERRNO_ENSRSERVFAIL            162 /* DNS server returned general failure */
#define LWIP_ERRNO_ENSRNOTFOUND            163 /* Domain name not found */
#define LWIP_ERRNO_ENSRNOTIMP              164 /* DNS server does not implement requested operation */
#define LWIP_ERRNO_ENSRREFUSED             165 /* DNS server refused query */
#define LWIP_ERRNO_ENSRBADQUERY            166 /* Misformatted DNS query */
#define LWIP_ERRNO_ENSRBADNAME             167 /* Misformatted domain name */
#define LWIP_ERRNO_ENSRBADFAMILY           168 /* Unsupported address family */
#define LWIP_ERRNO_ENSRBADRESP             169 /* Misformatted DNS reply */
#define LWIP_ERRNO_ENSRCONNREFUSED         170 /* Could not contact DNS servers */
#define LWIP_ERRNO_ENSRTIMEOUT             171 /* Timeout while contacting DNS servers */
#define LWIP_ERRNO_ENSROF                  172 /* End of file */
#define LWIP_ERRNO_ENSRFILE                173 /* Error reading file */
#define LWIP_ERRNO_ENSRNOMEM               174 /* Out of memory */
#define LWIP_ERRNO_ENSRDESTRUCTION         175 /* Application terminated lookup */
#define LWIP_ERRNO_ENSRQUERYDOMAINTOOLONG  176 /* Domain name is too long */
#define LWIP_ERRNO_ENSRCNAMELOOP           177 /* Domain name is too long */

#ifndef errno
extern int errno;
#endif

#endif /* LWIP_PROVIDE_ERRNO */

#ifdef __cplusplus
}
#endif

#endif /* __LWIP_ARCH_H__ */
