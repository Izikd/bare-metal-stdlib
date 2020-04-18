// SPDX-License-Identifier: MIT

#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "arm_uart.h"

char *__env[1] = {0};
char **environ = __env;

extern char heap_base;
extern char heap_top;
static char *heap_cur = &heap_base;

void *_sbrk(int increment)
{
	char *alloc_start;

	if ((heap_cur + increment) >= &heap_top) {
		errno = ENOMEM;

		return (void *)-1;
	}

	alloc_start = heap_cur;

	heap_cur += increment;

	return alloc_start;
}

int _write(int file,
	   char *ptr,
	   int len)
{
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))	{
		errno = EBADF;

		return -1;
	}

	arm_uart_nputs(ARM_UART_BASE, ptr, len);

	return len;
}

int _read(int file,
	   char *ptr,
	   int len)
{
	errno = ENOSYS;

	return -1;
}

int _lseek(int file,
	   int ptr,
	   int dir)
{
	errno = ENOSYS;

	return -1;
}

int _close(int fildes)
{
	errno = ENOSYS;

	return -1;
}

int _fstat(int fildes,
	   struct stat *st)
{
	errno = ENOSYS;

	return 0;
}

int _isatty(int file)
{
	errno = ENOSYS;

	return 1;
}
