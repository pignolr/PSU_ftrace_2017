/*
** EPITECH PROJECT, 2018
** ftrace
** File description:
** print_syscall.c
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "syscalls.h"

static int return_error(char *str)
{
	fprintf(stderr, "%s\n", str);
	return (ERR_RET);
}

static char *return_null()
{
	fprintf(stderr, "ERROR: Realloc in get_path\n");
	return (NULL);
}

static int check_if_exec(char *path, unsigned int i, const char *file)
{
	struct stat buff;

	memcpy(path + i, "/", 1);
	memcpy(path + i + 1, file, strlen(file));
	memcpy(path + i + 1 + strlen(file), "\0", 1);
	if (stat(path, &buff) != -1 && buff.st_mode & S_IXUSR)
		return (0);
	return (1);
}

static char *get_from_bin(char *env, const char *file)
{
	int save;
	unsigned int i;
	char *path;

	path = NULL;
	i = 0;
	for (save = i; save + i < strlen(env); save += i) {
		i = 0;
		while (save + i < strlen(env) && env[save + i] != ':')
			++i;
		path = realloc(path, i + strlen(file) + 2);
		if (path == NULL)
			return (return_null());
		memcpy(path, env + save, i);
		if (check_if_exec(path, i, file) == 0)
			return (path);
		++i;
	}
	free(path);
	return (NULL);
}

int get_path(elf_info_t *file_info)
{
	struct stat buff;
	char *env;

	if ((file_info->exec[0] == '.' || file_info->exec[0] == '/')
		&& stat(file_info->exec, &buff) != -1
		&& buff.st_mode & S_IXUSR) {
		file_info->path = strdup(file_info->exec + 2);
		if (file_info->path == NULL)
			return (return_error("ERROR: Malloc in get_path"));
		return (0);
	}
	env = getenv("PATH");
	if (env == NULL)
		return (ERR_RET);
	file_info->path = get_from_bin(env, file_info->exec);
	if (file_info->path == NULL)
		return (return_error("ERROR: Invalid or Inexistent file"));
	return (0);
}