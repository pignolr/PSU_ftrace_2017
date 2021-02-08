/*
** EPITECH PROJECT, 2018
** ftrace
** File description:
** main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ftrace.h"

int main(int ac, char **av, char **env)
{
	ftrace_args_t args;
	ftrace_t ftrace_info;

	memset(&ftrace_info, 0, sizeof(ftrace_t));
	if (get_arguments(&args, ac, av, env) == ERR_RET
		|| load_elf_file(&ftrace_info.file_info, &args)
		|| init_trace(&ftrace_info, &args) == ERR_RET
		|| ftrace(&ftrace_info) == ERR_RET)
		return (ERR_RET);
	unload_elf_file(&ftrace_info.file_info);
	return (EXIT_SUCCESS);
}
