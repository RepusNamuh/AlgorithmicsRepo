/* Implement and quality-test a good hash function.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/* These next would be in a header file for separate compilation */
#define NVALUES 20

typedef struct {
	unsigned nvalues;
	unsigned *values;
	unsigned tabsize;
} hashfunc_t;

/* A couple of auxiliary functions */
int
isprime(int n) {
	int divisor;
	if (n<2) {
		return 0;
	}
	for (divisor=2; divisor*divisor<=n; divisor++) {
	       if (n%divisor==0) {
		       /* factor found, so can't be prime */
		       return 0;
		}
	}
	/* no factors, so must be prime */
	return 1;
}

int
nextprime(int n) {
	n = n+1;
	while (!isprime(n)) {
		n = n+1;
	}
	return n;
}

/* Functions would be in a source code file for separate compilation */

hashfunc_t
*hash_func_create(unsigned tabsize) {
	int i;
	hashfunc_t *h;
	/* allocate the required memory space */
	h = malloc(sizeof(*h));
	assert(h != NULL);
	h->values = malloc(NVALUES*sizeof(*(h->values)));
	assert(h->values != NULL);
	h->nvalues = NVALUES;
	/* then create a sequence of prime numbers from it */
	for (i=0; i<NVALUES; i++) {
		/* assumes that srand() has already been called */
		h->values[i] = nextprime(tabsize + rand()%tabsize);
	}
	h->tabsize = tabsize;
	return h;
}

void
hash_func_free(hashfunc_t *h) {
	free(h->values);
	free(h);
}

unsigned
hash_func_calculate(hashfunc_t *h, char *key) {
	unsigned i, k=0, hval=0;
	/* first, process every character in the string */
	for (i=0; key[i]!='\0'; i++) {
		hval += key[i] * h->values[k];
		k += 1;
		if (k==NVALUES) {
			k = 0;
		}
	}
	/* then reduce into the desired range */
	return hval % h->tabsize;
}

/* Make use of some scaffolding to test these functions */
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