// SPDX-License-Identifier: MIT

/**
 * Uses these sources as a reference or a guide:
 * - https://www.embecosm.com/appnotes/ean9/ean9-howto-newlib-1.0.html
 * - https://github.com/bminor/newlib/tree/master/libgloss/libnosys
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <_syslist.h>

#include "arm_uart.h"

char *__env[1] = {0};
char **environ = __env;

extern char heap_base;
extern char heap_top;
static char *heap_cur = &heap_base;

void _exit_asm(int rc) __attribute__((noreturn));

void _exit(int rc)
{
	printf("exit(%d)\n", rc);

	_exit_asm(rc);
}

int _getpid(void)
{
	/**
	 * This is a single process.
	 * Value '0' might be some magic PID for some applications,
	 * i.e. the root process.
	 */
	return 1;
}

int _kill(int pid, int sig)
{
	errno = EINVAL;

	return -1;
}

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
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
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
	int i;
	char c = 0;

	if (file != STDIN_FILENO) {
		errno = EBADF;

		return -1;
	}

	/* Stop at end of buffer or if previous char was new line */
	for (i = 0; (i < len) && (c != '\n'); i++) {
		c = arm_uart_getc(ARM_UART_BASE);

		/* Fixup */
		if (c == '\r')
			c = '\n';

		/* Echo back */
		arm_uart_putc(ARM_UART_BASE, c);

		ptr[i] = c;
	}

	return i;
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
