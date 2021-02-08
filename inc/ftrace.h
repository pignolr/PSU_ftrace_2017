/*
** EPITECH PROJECT, 2018
** ftrace
** File description:
** ftrace.h
*/

#ifndef ftrace_ftrace_H_
#define ftrace_ftrace_H_

#include <stdbool.h>
#include <sys/types.h>
#include <sys/user.h>
#include <gelf.h>
#include <libelf.h>
#define USAGE "USAGE: %s <command>\n"
#define ERR_RET 84
#define ARGS "+h"

#define OPCODE_SYSCALL 0x050f
#define OPCODE_FUNCCALL 0xe8
#define OPCODE_RETURN(op) ((op & 0xFF) == 0xC2  || (op & 0xFF) == 0xC3 \
				|| (op & 0xFF) == 0xCA || (op & 0xFF) == 0xCB)

typedef enum reg_args_e {
	REG_RDI_IDX = 0,
	REG_RSI_IDX = 1,
	REG_RDX_IDX = 2,
	REG_RCX_IDX = 3,
	REG_R8_IDX = 4,
	REG_R9_IDX = 5,
	REG_MAX_IDX = 6
} reg_args_t;

typedef struct elf_info_s {
	const char *exec;
	char *path;
	int fd;
	Elf *hdr;
	Elf_Data *data;
	GElf_Shdr shdr;
	size_t nb_sym;
} elf_info_t;

typedef struct ftrace_args_s {
	char **command;
	char **env;
} ftrace_args_t;

typedef struct ftrace_s {
	elf_info_t file_info;
	pid_t target_pid;
	int status;
	struct user_regs_struct regs;
} ftrace_t;

int get_arguments(ftrace_args_t *args, int ac, char **av, char **env);
int load_elf_file(elf_info_t *file_info, ftrace_args_t *args);
void unload_elf_file(elf_info_t *file_info);
int get_path(elf_info_t *file_info);
int init_trace(ftrace_t *ftrace_info, ftrace_args_t *args);

int analyse_step(ftrace_t *ftrace_info);
int do_single_step(ftrace_t *ftrace_info);
int analyse_status(ftrace_t *ftrace_info);

int ftrace(ftrace_t *ftrace_info);
int print_func_call(ftrace_t *ftrace_info);
int print_syscall(ftrace_t *ftrace_info);
int print_exit(ftrace_t *ftrace_info);

#endif /* !ftrace_ftrace_H_ */
