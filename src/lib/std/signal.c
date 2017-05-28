/**
 * @file include/signal.c
 * Standard signal definitions.
 * @author Conlan Wesson
 */

#include <signal.h>
#include <errno.h>
#include <kernel/panic.h>

/**
 * Default abort signal handler.
 * @param signal Signal invoking handler.
 */
void sighandler_abort(int signal){
	(void)signal;
	panic("Signal Abort");
}

/**
 * Default exit signal handler.
 * @param signal Signal invoking handler.
 */
void sighandler_exit(int signal){
	(void)signal;
	panic("Signal Terminate");
}

/**
 * Default ignore signal handler.
 * @param signal Signal invoking handler.
 */
void sighandler_ignore(int signal){
	(void)signal;
}

/**
 * Default stop signal handler.
 * @param signal Signal invoking handler.
 */
void sighandler_stop(int signal){
	(void)signal;
}

/**
 * Default continue signal handler.
 * @param signal Signal invoking handler.
 */
void sighandler_cont(int signal){
	(void)signal;
}

/**
 * Default signal handler.
 * @param signal Signal invoking handler.
 */
void sighandler_default(int signal){
	switch(signal){
		case SIGNULL:
		case SIGCHLD:
		case SIGURG:
			sighandler_ignore(signal);
			break;
		case SIGABRT:
		case SIGFPE:
		case SIGILL:
		case SIGSEGV:
		case SIGBUS:
		case SIGQUIT:
		case SIGSYS:
		case SIGTRAP:
		case SIGXCPU:
		case SIGXFSZ:
			sighandler_abort(signal);
			break;
		case SIGINT:
		case SIGTERM:
		case SIGALRM:
		case SIGHUP:
		case SIGKILL:
		case SIGPIPE:
		case SIGUSR1:
		case SIGUSR2:
		case SIGPOLL:
		case SIGPROF:
		case SIGVTALRM:
			sighandler_exit(signal);
			break;
		case SIGCONT:
			sighandler_cont(signal);
			break;
		case SIGSTOP:
		case SIGTSTP:
		case SIGTTIN:
		case SIGTTOU:
			sighandler_stop(signal);
			break;
		default:
			sighandler_ignore(signal);
			break;
	}
}

//! Signal handlers
static void (*sighandler[SIGRTMAX+1])(int) = {
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
	sighandler_default,
};

/**
 * Raise a signal.
 * @param signal Signal to raise.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int raise(int signal){
	sighandler[signal](signal);
	return EOK;
}

/**
 * Set signal handler for the given signal.
 * @param signal Signal to handle.
 * @param handler New handler function
 */
void signal(int sig, void (*handler)(int)){
	sighandler[sig] = handler;
}

/**
 * Add a signal to the set.
 * @param set Set to add to.
 * @param signal Signal to add.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigaddset(sigset_t* set, int signal){
	int ret = SIG_ERR;
	if(signal > SIGNULL && signal <= SIGRTMAX){
		*set |= (1 << (signal-1));
		ret = EOK;
	}
	return ret;
}

/**
 * Delete a signal from the set.
 * @param set Set to delete to.
 * @param signal Signal to delete.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigdelset(sigset_t* set, int signal){
	int ret = SIG_ERR;
	if(signal > SIGNULL && signal <= SIGRTMAX){
		*set &= ~(1 << (signal-1));
		ret = EOK;
	}
	return ret;
}

/**
 * Clear all signals from the set.
 * @param set Set to empty.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigemptyset(sigset_t* set){
	*set = 0;
	return EOK;
}

/**
 * Set all signals from the set.
 * @param set Set to fill.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int sigfillset(sigset_t* set){
	*set = -1;
	return EOK;
}

