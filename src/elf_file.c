/*
** EPITECH PROJECT, 2021
** ftrace
** File description:
** elf_file.c
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ftrace.h"

static int	get_symtab(elf_info_t *file_info)
{
	Elf_Scn	*scn;

	for (scn = elf_nextscn(file_info->hdr, NULL); scn != NULL;
		scn = elf_nextscn(file_info->hdr, scn)) {
		if (!gelf_getshdr(scn, &file_info->shdr))
			return (1);
		if (file_info->shdr.sh_type == SHT_SYMTAB) {
			if (file_info->shdr.sh_entsize == 0)
				continue;
			file_info->nb_sym = file_info->shdr.sh_size
				/ file_info->shdr.sh_entsize;
			file_info->data = elf_getdata(scn, NULL);
			return (0);
		}
	}
	return (1);
}

int load_elf_file(elf_info_t *file_info, ftrace_args_t *args)
{
	if (elf_version(EV_CURRENT) == EV_NONE) {
		fprintf(stderr, "ELF library too old\n");
		return (ERR_RET);
	}
	file_info->exec = args->command[0];
	if (get_path(file_info))
		return (ERR_RET);
	file_info->fd = open(file_info->path, O_RDONLY);
	if (file_info->fd == -1)
		return (ERR_RET);
	file_info->hdr = elf_begin(file_info->fd, ELF_C_READ, NULL);
	if (file_info->hdr == NULL)
		return (0);
	if (elf_kind(file_info->hdr) != ELF_K_ELF
		|| gelf_getclass(file_info->hdr) == ELFCLASSNONE) {
		fprintf(stderr, "This file is not an ELF file\n");
		return (ERR_RET);
	}
	get_symtab(file_info);
	return (0);
}

void unload_elf_file(elf_info_t *file_info)
{
	if (file_info->hdr)
		elf_end(file_info->hdr);
	close(file_info->fd);
	free(file_info->path);
}
