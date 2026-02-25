/* Implement and quality-test a (bad) hash function.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
	unsigned tabsize;
} hashfunc_t;

hashfunc_t
*hash_func_create(unsigned tabsize) {
	hashfunc_t *h;
	/* allocate the required memory space */
	h = malloc(sizeof(*h));
	assert(h != NULL);
	h->tabsize = tabsize;
	return h;
}

void
hash_func_free(hashfunc_t *h) {
	free(h);
}

/* BEWARE: Dangerous code. Don't do this! */
unsigned
hash_func_calculate(hashfunc_t *h, char *key) {
	unsigned i, hval=0;
	for (i=0; key[i]!='\0'; i++) {
		hval = hval + (i+1)*key[i];
	}
	return hval % h->tabsize;
}

#include "hashscaffold.c"

/* =====================================================================
   Program written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */