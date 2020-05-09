// SPDX-License-Identifier: MIT

#include <stdio.h>
#include "assert.h"

int main(void)
{
	int x;
	float f;
	int rc;

	setvbuf

	/* Integer */
	printf("Enter an integer: ");
	fflush(stdout);

	rc = scanf("%d", &x);
	assert(rc == 1);

	printf("You've entered: %d\n", x);

	/* Float */
	printf("Enter an float: ");
	fflush(stdout);

	rc = scanf("%f", &f);
	assert(rc == 1);

	printf("You've entered: %f\n", f);

	return 0;
}
