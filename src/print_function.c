/*
** EPITECH PROJECT, 2018
** ftrace
** File description:
** print_function.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include "syscalls.h"

static char *get_name(ftrace_t *ftrace_info, unsigned long reg)
{
	GElf_Sym table;
	unsigned int i;

	if (!ftrace_info->file_info.shdr.sh_entsize)
		return (NULL);
	for (i = 0; i < (ftrace_info->file_info.shdr.sh_size
		/ ftrace_info->file_info.shdr.sh_entsize); ++i) {
		gelf_getsym(ftrace_info->file_info.data, i, &table);
		if (table.st_value == reg)
			return (elf_strptr(ftrace_info->file_info.hdr,
				ftrace_info->file_info.shdr.sh_link,
				table.st_name));
	}
	return (NULL);
}

static int print_entering(ftrace_t *ftrace_info, unsigned long reg)
{
	char *name;

	name = get_name(ftrace_info, reg);
	if (name != NULL) {
		fprintf(stderr, "Entering function %s at 0x%lx\n", name, reg);
		free(name);
	}
	else
		fprintf(stderr, "Entering func_0x%lx@%s\n", reg,
			ftrace_info->file_info.path);
	return (0);
}

static int print_leaving(ftrace_t *ftrace_info, unsigned long reg)
{
	char *name;

	name = get_name(ftrace_info, reg);
	if (name != NULL) {
		fprintf(stderr, "Leaving function %s at 0x%lx\n", name, reg);
		free(name);
	}
	else
		fprintf(stderr, "Leaving func_0x%lx@%s\n", reg,
			ftrace_info->file_info.path);
	return (0);
}

static bool get_function_end(ftrace_t *ftrace_info, unsigned long addr)
{
	unsigned short ret;

	ret = ptrace(PTRACE_PEEKTEXT, ftrace_info->target_pid,
		ftrace_info->regs.rip, NULL);
	while (WIFSTOPPED(ftrace_info->status) && OPCODE_RETURN(ret) != 0)
		if (analyse_step(ftrace_info))
			return (1);
	if (OPCODE_RETURN(ret) == 0) {
		print_leaving(ftrace_info, addr);
		return (1);
	}
	return (0);
}

int print_func_call(ftrace_t *info)
{
	unsigned long	op;
	unsigned long	addr;
	char		rex;

	op = ptrace(PTRACE_PEEKTEXT, info->target_pid, info->regs.rip, NULL);
	rex = 0;
	if ((op & 0xF0) == 0x40) {
		rex = op & 0x8;
		op = ptrace(PTRACE_PEEKTEXT, info->target_pid,
			++info->regs.rip, NULL);
	}
	if (rex)
		addr = info->regs.rip + ptrace(PTRACE_PEEKTEXT,
			info->target_pid, info->regs.rip + 1) + 9;
	else
		addr = info->regs.rip + (int)((op >> 8)) + 5;
	print_entering(info, addr);
	while (get_function_end(info, addr) == 0)
		;
	return (0);
}