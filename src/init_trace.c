/*
** EPITECH PROJECT, 2021
** ftrace
** File description:
** attach.c
*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include "ftrace.h"

static void exec_command(char **command, char **env)
{
	if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
		perror("Error ptrace PTRACE_TRACEME");
		exit(ERR_RET);
	}
	if (execvpe(command[0], command, env) == -1) {
		perror("Error execvp");
		exit(ERR_RET);
	}
}

static int trace_command(ftrace_t *ftrace_info, ftrace_args_t *args)
{
	if (waitpid(ftrace_info->target_pid, &ftrace_info->status, 0) == -1) {
		perror("Error waitpid");
		return (ERR_RET);
	}
	if (ptrace(PTRACE_SETOPTIONS, ftrace_info->target_pid, NULL,
		PTRACE_O_TRACEEXEC | PTRACE_O_TRACEEXIT) == -1) {
		perror("Error ptrace PTRACE_OPTION");
		return (ERR_RET);
	}
	fprintf(stderr, "execve(%p, %p, %p) = 0x0\n",
		args->command[0], args->command, args->env);
	return (0);
}

int init_trace(ftrace_t *ftrace_info, ftrace_args_t *args)
{
	ftrace_info->target_pid = fork();
	if (ftrace_info->target_pid == -1) {
		perror("Error fork");
		return (ERR_RET);
	} else if (ftrace_info->target_pid != 0) {
		return (trace_command(ftrace_info, args));
	} else {
		exec_command(args->command, args->env);
		return (0);
	}
}
