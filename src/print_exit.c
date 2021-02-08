/*
** EPITECH PROJECT, 2021
** ftrace
** File description:
** print_exit.c
*/

#include <stdio.h>
#include <sys/ptrace.h>
#include "ftrace.h"

int print_exit(ftrace_t *ftrace_info)
{
	long ret;

	if (ptrace(PTRACE_GETEVENTMSG, ftrace_info->target_pid, NULL, &ret)
		== -1) {
		perror("Error ptrace PTRACE_GETEVENTMSG");
		return (ERR_RET);
	}
	fprintf(stderr, "+++ exited with %d +++\n", (char)ret);
	return (0);
}
