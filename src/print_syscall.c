/*
** EPITECH PROJECT, 2018
** ftrace
** File description:
** print_syscall.c
*/

#include <stdio.h>
#include "syscalls.h"

int print_hex(ftrace_t *ftrace_info __attribute__((unused)), unsigned long reg)
{
	return (fprintf(stderr, "0x%lx", reg));
}

int rough_print(ftrace_t *ftrace_info, unsigned long *regs)
{
	int i;

	fprintf(stderr, "Syscall %s(",
		syscall_tab_g[ftrace_info->regs.orig_rax].name);
	for (i = 0; i < syscall_tab_g[ftrace_info->regs.orig_rax].n_args;
		++i) {
		if (print_hex(ftrace_info, regs[i]) < 0)
			return (ERR_RET);
		if (i + 1 < syscall_tab_g[ftrace_info->regs.orig_rax].n_args)
			fprintf(stderr, ", ");
	}
	fprintf(stderr, ") = ");
	if (syscall_tab_g[ftrace_info->regs.orig_rax].ret == VOID)
		fprintf(stderr, "?");
	else if (print_hex(ftrace_info, ftrace_info->regs.rax) < 0)
		return (ERR_RET);
	fprintf(stderr, "\n");
	return (0);
}

int print_syscall(ftrace_t *ftrace_info)
{
	unsigned long regs[REG_MAX_IDX];

	regs[REG_RDI_IDX] = ftrace_info->regs.rdi;
	regs[REG_RSI_IDX] = ftrace_info->regs.rsi;
	regs[REG_RDX_IDX] = ftrace_info->regs.rdx;
	regs[REG_RCX_IDX] = ftrace_info->regs.rcx;
	regs[REG_R8_IDX] = ftrace_info->regs.r8;
	regs[REG_R9_IDX] = ftrace_info->regs.r9;
	return (rough_print(ftrace_info, regs));
}