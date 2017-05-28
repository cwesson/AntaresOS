/**
 * @file include/signal.h
 * Standard signal definitions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_SIGNAL_H_
#define __INCLUDE_SIGNAL_H_

#include <limits.h>

//! Integer type for signals.
enum {
	SIGNULL = 0, //!< Not a signal
	
	// ISO C signals
	SIGABRT,   //!< Process abort signal
	SIGFPE,    //!< Erroneous arithmetic operation
	SIGILL,    //!< Illegal instruction
	SIGINT,    //!< Termal interrupt signal
	SIGSEGV,   //!< Invalid memory reference
	SIGTERM,   //!< Terminaion signal
	
	// POSIX signals
	SIGALRM,   //!< Alarm clock
	SIGBUS,    //!< Access to an undefined portion of a memory object
	SIGCHLD,   //!< Child process terminated, stopped, or continued
	SIGCONT,   //!< Continue executing, if stopped
	SIGHUP,    //!< Hangup
	SIGKILL,   //!< Kill (cannot be caught or ignored)
	SIGPIPE,   //!< Write on a pipe with no one to read it
	SIGQUIT,   //!< Terminal quit signal
	SIGSTOP,   //!< Stop executing (cannot be caught or ignored)
	SIGTSTP,   //!< Terminal stop signal
	SIGTTIN,   //!< Background process attempting read
	SIGTTOU,   //!< Background process attempting write
	SIGUSR1,   //!< User-defined signal 1
	SIGUSR2,   //!< User-defined signal 2
	SIGPOLL,   //!< Pollable event
	SIGPROF,   //!< Profiling timer expired
	SIGSYS,    //!< Bad system call
	SIGTRAP,   //!< Trace/breakpoint trap
	SIGURG,    //!< High bandwidth data is available at a socket
	SIGVTALRM, //!< Virtual timer expired
	SIGXCPU,   //!< CPU time limit exceeded
	SIGXFSZ,   //!< File size limit exceeded
	
	SIGRTMIN,
	SIGRTMAX = SIGRTMIN + RTSIG_MAX
};

enum {
	ILL_ILLOPC,
	ILL_ILLOPN,
	ILL_ILLADDR,
	ILL_ILLTRP,
	ILL_PRVOPC,
	ILL_PRVREG,
	ILL_COPROC,
	ILL_BADSTK,
	FPE_INTDIV,
	FPE_INTOVF,
	FPE_FLTDIV,
	FPE_FLTOVF,
	FPE_FLTUND,
	FPE_FLTRES,
	FPE_FLTINV,
	FPE_FLTSUB,
	SEGV_MAPERR,
	SEGV_ACCERR,
	BUS_ADRALN,
	BUS_ADRERR,
	BUS_OBJERR,
	TRAP_BRKPT,
	TRAP_TRACE,
	CLD_EXITED,
	CLD_KILLED,
	CLD_DUMPED,
	CLD_TRAPPED,
	CLD_STOPPED,
	CLD_CONTINUED,
	POLL_IN,
	POLL_OUT,
	POLL_MSG,
	POLL_ERR,
	POLL_PRI,
	POLL_HUP,
	SI_USER,
	SI_QUEUE,
	SI_TIMER,
	SI_ASYNCIO,
	SI_MESGQ,
};

/**
 * Request for default signal handling.
 * @param signal Signal to handle.
 */
void SIG_DFL(int signal);

//! Return value from signal() in case of error.
enum {
	SIG_ERR = -1
};

/**
 * Request that signal be held.
 * @param signal Signal to handle.
 */
void SIG_HOLD(int signal);

/**
 * Request that signal be ignored.
 * @param signal Signal to handle.
 */
void SIG_IGN(int signal);

typedef int sig_atomic_t;
typedef unsigned long long sigset_t;

_Static_assert(sizeof(sigset_t)*8 >= SIGRTMAX, "sigset_t too small");

union sigval {
	int   sival_int;
	void* sival_ptr;
};

struct sigevent {
	int          sigev_notify;
	int          sigev_signo;
	union sigval sigev_value;
	void (*sigev_notify_function)(union sigval);
};

//! Values for sigev_notify
enum {
	SIGEV_NONE,
	SIGEV_SIGNAL,
	SIGEV_THREAD,
};

typedef struct {
	int   si_signo;
	int   si_code;
	int   si_errno;
	void* si_addr;
	int   si_status;
	long  si_band;
	union sigval si_val;
} siginfo_t;

struct sigaction {
	void (*sa_handler)(int);
	sigset_t sa_mask;
	int sa_flags;
	void (*sa_sigaction)(int, siginfo_t*, void*);
};

/**
 * Raise a signal.
 * @param signal Signal to raise.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int raise(int signal);

/**
 * Set signal handler for the given signal.
 * @param sig Signal to handle.
 * @param handler New handler function
 */
void signal(int sig, void (*handler)(int));

/**
 * Add a signal to the set.
 * @param set Set to add to.
 * @param signal Signal to add.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigaddset(sigset_t* set, int signal);

/**
 * Delete a signal from the set.
 * @param set Set to delete to.
 * @param signal Signal to delete.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigdelset(sigset_t* set, int signal);

/**
 * Clear all signals from the set.
 * @param set Set to empty.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigemptyset(sigset_t* set);

/**
 * Set all signals from the set.
 * @param set Set to fill.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigfillset(sigset_t* set);

#endif

