/**
 * @file include/errno.h
 * Standard error code definitions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_ERRNO_H_
#define __INCLUDE_ERRNO_H_

//! Integer type used for indicating errors.
typedef enum {
	EOK = 0,         //!< Not an error.
	
	// ISO C required errors.
	EDOM,            //!< Mathematics argument out of domain of function.
	ERANGE,          //!< Result outside of function's range.
	EILSEQ,          //!< Illegal byte sequence.
	
	// POSIX required errors.
	E2BIG,           //!< Argument list too long.
	EACCES,          //!< Permission denied.
	EADDRINUSE,      //!< Address in use.
	EADDRNOTAVAIL,   //!< Address not available.
	EAFNOSUPPORT,    //!< Address family not supported.
	EAGAIN,          //!< Resource unavailable, try again (may be the same value as [EWOULDBLOCK]).
	EALREADY,        //!< Connection already in progress.
	EBADF,           //!< Bad file descriptor.
	EBADMSG,         //!< Bad message.
	EBUSY,           //!< Device or resource busy.
	ECANCELED,       //!< Operation canceled.
	ECHILD,          //!< No child processes.
	ECONNABORTED,    //!< Connection aborted.
	ECONNREFUSED,    //!< Connection refused.
	ECONNRESET,      //!< Connection reset.
	EDEADLK,         //!< Resource deadlock would occur.
	EDESTADDRREQ,    //!< Destination address required.
	EDQUOT,          //!< Reserved.
	EEXIST,          //!< File exists.
	EFAULT,          //!< Bad address.
	EFBIG,           //!< File too large.
	EHOSTUNREACH,    //!< Host is unreachable.
	EIDRM,           //!< Identifier removed.
	EINPROGRESS,     //!< Operation in progress.
	EINTR,           //!< Interrupted function.
	EINVAL,          //!< Invalid argument.
	EIO,             //!< I/O error.
	EISCONN,         //!< Socket is connected.
	EISDIR,          //!< Is a directory.
	ELOOP,           //!< Too many levels of symbolic links.
	EMFILE,          //!< Too many open files.
	EMLINK,          //!< Too many links.
	EMSGSIZE,        //!< Message too large.
	EMULTIHOP,       //!< Reserved.
	ENAMETOOLONG,    //!< Filename too long.
	ENETDOWN,        //!< Network is down.
	ENETRESET,       //!< Connection aborted by network.
	ENETUNREACH,     //!< Network unreachable.
	ENFILE,          //!< Too many files open in system.
	ENOBUFS,         //!< No buffer space available.
	ENODATA,         //!< No message is available on the STREAM head read queue.
	ENODEV,          //!< No such device.
	ENOENT,          //!< No such file or directory.
	ENOEXEC,         //!< Executable file format error.
	ENOLCK,          //!< No locks available.
	ENOLINK,         //!< Reserved.
	ENOMEM,          //!< Not enough space.
	ENOMSG,          //!< No message of the desired type.
	ENOPROTOOPT,     //!< Protocol not available.
	ENOSPC,          //!< No space left on device.
	ENOSR,           //!< No STREAM resources.
	ENOSTR,          //!< Not a STREAM.
	ENOSYS,          //!< Function not supported.
	ENOTCONN,        //!< The socket is not connected.
	ENOTDIR,         //!< Not a directory.
	ENOTEMPTY,       //!< Directory not empty.
	ENOTSOCK,        //!< Not a socket.
	ENOTSUP,         //!< Not supported.
	ENOTTY,          //!< Inappropriate I/O control operation.
	ENXIO,           //!< No such device or address.
	EOPNOTSUPP,      //!< Operation not supported on socket.
	EOVERFLOW,       //!< Value too large to be stored in data type.
	EPERM,           //!< Operation not permitted.
	EPIPE,           //!< Broken pipe.
	EPROTO,          //!< Protocol error.
	EPROTONOSUPPORT, //!< Protocol not supported.
	EPROTOTYPE,      //!< Protocol wrong type for socket.
	EROFS,           //!< Read-only file system.
	ESPIPE,          //!< Invalid seek.
	ESRCH,           //!< No such process.
	ESTALE,          //!< Reserved.
	ETIME,           //!< Stream ioctl() timeout.
	ETIMEDOUT,       //!< Connection timed out.
	ETXTBSY,         //!< Text file busy.
	EWOULDBLOCK,     //!< Operation would block (may be the same value as [EAGAIN]).
	EXDEV            //!< Cross-device link.
} errno_t;

//! Stores the currnet error state.
errno_t errno;

#endif /* __INCLUDE_ERRNO_H_ */

