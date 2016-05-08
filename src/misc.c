#ifndef _MISC_C_
#define _MISC_C_

#include <stdio.h>
#include "misc.h"

#define PADDING_STR "\t"

void print_padding(FILE* dest, int count) {

	for (int i = 0; i < count; i++) {
		fprintf(dest, PADDING_STR);
	}
}

#endif
