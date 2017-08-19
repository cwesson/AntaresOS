/**
 * @file signal.c
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
void SIG_IGN(int signal){
	(void)signal;
}

/**
 * Default hold signal handler.
 * @param signal Signal invoking handler.
 */
void SIG_HOLD(int signal){
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
void SIG_DFL(int signal){
	switch(signal){
		case SIGNULL:
		case SIGCHLD:
		case SIGURG:
			SIG_IGN(signal);
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
			SIG_IGN(signal);
			break;
	}
}

//! Signal handlers
static void (*sighandler[SIGRTMAX+1])(int) = {
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
	SIG_DFL,
};

static sigset_t sigmask = 0;

/**
 * Raise a signal.
 * @param signal Signal to raise.
 * @return Zero is successful, SIG_ERR otherwise.
 */
int raise(int signal){
	if(!sigismember(&sigmask, signal)){
		sighandler[signal](signal);
	}
	return EOK;
}

/**
 * Set signal handler for the given signal.
 * @param signal Signal to handle.
 * @param handler New handler function.
 * @return Previous signal handler.
 */
void (*signal(int sig, void (*handler)(int)))(int){
	void (*old)(int) = sighandler[sig];
	if(handler == SIG_HOLD){
		sighold(sig);
	}else{
		sighandler[sig] = handler;
		
		if(sigismember(&sigmask, sig)){
			old = SIG_HOLD;
			sigrelse(sig);
		}
	}
	return old;
}

/**
 * Request that signal be held.
 * @param sig Signal to handle.
 */
void sighold(int sig){
	sigaddset(&sigmask, sig);
}

/**
 * Request that signal no longer be held.
 * @param sig Signal to handle.
 */
void sigrelse(int sig){
	sigdelset(&sigmask, sig);
}

/**
 * Request that signal be ignored.
 * @param sig Signal to handle.
 */
void sigignore(int sig){
	signal(sig, SIG_IGN);
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
		*set |= (1ull << (signal-1));
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
		*set &= ~(1ull << (signal-1));
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

/**
 * Checks if the signal is in the set.
 * @param set Set to check.
 * @param signal Signal to check.
 * @return 1 is signal is set, 0 otherwise.
 */
int sigismember(const sigset_t *set, int signal){
	return !!(*set & (1ull << (signal-1)));
}

