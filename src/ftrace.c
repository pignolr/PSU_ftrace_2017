/*
** EPITECH PROJECT, 2021
** ftrace
** File description:
** ftrace.c
*/

#include <stdio.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "ftrace.h"

int do_single_step(ftrace_t *ftrace_info)
{
	if (ptrace(PTRACE_SINGLESTEP, ftrace_info->target_pid, NULL, NULL)
		== -1) {
		perror("Error ptrace PTRACE_SINGLESTEP");
		return (ERR_RET);
	}
	if (waitpid(ftrace_info->target_pid, &ftrace_info->status, 0) == -1) {
		perror("Error waitpid");
		return (ERR_RET);
	}
	return (0);
}

int analyse_step(ftrace_t *ftrace_info)
{
	if (analyse_status(ftrace_info) == ERR_RET)
		return (ERR_RET);
	if (do_single_step(ftrace_info) == ERR_RET)
		return (ERR_RET);
	return (0);
}

int ftrace(ftrace_t *ftrace_info)
{
	while (WIFSTOPPED(ftrace_info->status)) {
		if (analyse_step(ftrace_info) == ERR_RET)
			return (ERR_RET);
	}
	return (0);
}
