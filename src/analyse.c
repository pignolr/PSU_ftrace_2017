/*
** EPITECH PROJECT, 2021
** ftrace
** File description:
** analyse.c
*/

#include <stdio.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "ftrace.h"
#include "signals.h"

static int analyse_opcode(ftrace_t *ftrace_info)
{
	if (do_single_step(ftrace_info))
		return (ERR_RET);
	if (!WIFSTOPPED(ftrace_info->status))
		return (0);
	if (ptrace(PTRACE_GETREGS, ftrace_info->target_pid, NULL,
		&ftrace_info->regs)) {
		perror("Error ptrace PTRACE_GETREGS");
		return (ERR_RET);
	}
	return (print_syscall(ftrace_info));
}

static int analyse_trap(ftrace_t *ftrace_info)
{
	unsigned short ret;

	if (ptrace(PTRACE_GETREGS, ftrace_info->target_pid, NULL,
		&ftrace_info->regs)) {
		perror("Error ptrace PTRACE_GETREGS");
		return (ERR_RET);
	}
	ret = (unsigned short)ptrace(PTRACE_PEEKTEXT, ftrace_info->target_pid,
		ftrace_info->regs.rip, NULL);
	if (errno != 0) {
		perror("Error ptrace PTRACE_PEEKTEXT");
		return (ERR_RET);
	}
	if (ret == OPCODE_SYSCALL)
		return (analyse_opcode(ftrace_info));
	else
		return (0);
}

static int analyse_signal(ftrace_t *ftrace_info)
{
	siginfo_t signal_info;

	if (ptrace(PTRACE_GETSIGINFO, ftrace_info->target_pid, NULL,
		&signal_info)) {
		perror("Error ptrace PTRACE_PEEKTEXT");
		return (ERR_RET);
	}
	if (signal_info.si_signo != SIGTRAP) {
		if (signal_info.si_signo <= MAX_SYSCALL)
			printf("Received signal %s\n",
				signal_tab_g[signal_info.si_signo]);
		else
			printf("Received signal %d\n", signal_info.si_signo);
	}
	return (0);
}

int analyse_status(ftrace_t *ftrace_info)
{
	if (analyse_signal(ftrace_info))
		return (1);
	if (ftrace_info->status >> 8 == (SIGTRAP | (PTRACE_EVENT_EXEC << 8)))
		return (print_syscall(ftrace_info));
	if (WSTOPSIG(ftrace_info->status) == SIGTRAP)
		return (analyse_trap(ftrace_info));
	if (ftrace_info->status >> 16 == PTRACE_EVENT_EXIT)
		return (print_exit(ftrace_info));
	return (0);
}
