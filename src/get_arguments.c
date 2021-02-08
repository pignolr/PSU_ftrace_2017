/*
** EPITECH PROJECT, 2021
** ftrace
** File description:
** get_arguments.c
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include "ftrace.h"

int get_arguments(ftrace_args_t *args, int ac, char **av, char **env)
{
	struct option opt_long[1];
	int c;

	if (ac == 1) {
		printf(USAGE, av[0]);
		return (ERR_RET);
	}
	memset(args, 0, sizeof(ftrace_args_t));
	opt_long[0] = (struct option){ "help", 0, NULL, 'h' };
	for (c = getopt_long(ac, av, ARGS, opt_long, NULL); c != -1;
		c = getopt_long(ac, av, ARGS, opt_long, NULL)) {
		if (c == 'h') {
			printf(USAGE, av[0]);
			exit(EXIT_SUCCESS);
		}
	}
	if (optind < ac)
		args->command = av + optind;
	args->env = env;
	return (EXIT_SUCCESS);
}
