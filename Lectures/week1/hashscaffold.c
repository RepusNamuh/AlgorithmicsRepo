/* Example of use of hash table to identify unique strings.
*/

/* All standard #include lines are in the other file that
   includes this one. Done this way to allow easy switching between
   hash modules, but wouldn't normally be like this */

/* defines for the "main program" */

#define HTAB_DEFAULT 10000
#define WORDLEN 100
#define FIRST_SIZE 4

/* Set up an element type */
typedef char *data_t;


/* And a global variable to count string comparisons */
int tot_cmps=0;

/* the next block of code should be in a module hash_table.h */

typedef struct {
	int bucket_limit;
	int bucket_size;
	data_t *ptrs;
} hash_bucket_t;

typedef struct {
	int htab_size;
	hash_bucket_t *buckets;
	hashfunc_t *hash;
} hash_table_t;

/* and this next block in a corresponding hash_table.c file */

hash_table_t
*hash_table_create(int size) {
	int i;
	hash_table_t *H;
	H = malloc(sizeof(*H));
	assert(H);
	H->htab_size = size;
	H->buckets  = malloc(size*sizeof(hash_bucket_t));
	assert(H->buckets) ;
	for (i=0; i<size; i++) {
		H->buckets[i].bucket_size = 0;
		H->buckets[i].bucket_limit = 0;
		H->buckets[i].ptrs = NULL;
	}
	H->hash = hash_func_create(size);
	return H;
}

/* search for an item, if found, return its addres */
data_t
hash_table_search(hash_table_t *H, data_t key, int cmp()) {
	int i;
	unsigned hash;
	hash_bucket_t *p;
	hash = hash_func_calculate(H->hash, key);
	p = H->buckets + hash;
	/* search indicated bucket */
	for (i=0; i<p->bucket_size; i++) {
		tot_cmps += 1;
		assert(p->ptrs[i]);
		if (cmp(key, p->ptrs[i])==0) {
			return p->ptrs[i];
		}
	}
	/* wasn't found in the bucket */
	return NULL;
}

/* insert an item */
hash_table_t
*hash_table_insert(hash_table_t *H, data_t key) {
	hash_bucket_t *p;
	unsigned hash;
	hash = hash_func_calculate(H->hash, key);
	p = H->buckets + hash;
	if (p->bucket_limit == 0) {
		/* first use of this bucket */
		p->bucket_limit = FIRST_SIZE;
		p->ptrs = malloc(p->bucket_limit*sizeof(data_t));
		assert(p->ptrs);
	}
	if (p->bucket_size == p->bucket_limit) {
		p->bucket_limit *= 2;
		p->ptrs = realloc(p->ptrs,
			p->bucket_limit*sizeof(data_t));
		assert(p->ptrs);
	}
	/* definitely space there now */
	p->ptrs[p->bucket_size] = key;
	p->bucket_size++;
	return H;
}

/* nuke the hash table and contents */
void
hash_table_free(hash_table_t *H) {
	hash_bucket_t *p;
	int i, j;
	for (i=0; i<H->htab_size; i++) {
		p = H->buckets + i;
		if (p->ptrs) {
			for (j=0; j<p->bucket_size; j++) {
				free(p->ptrs[j]);
			}
			free(p->ptrs);
		}
	}
	free(H->buckets);
	hash_func_free(H->hash);
	free(H);
}

/* make use of the getword function */
#include "getword.c"

int
main(int argc, char *argv[]) {
	hash_table_t *H;
	char word[WORDLEN+1];
	char *new;
	int htab_size=HTAB_DEFAULT;
	int i, dwords=0, nwords=0, max, maxloc, min, mincount;
	int found;

	srand(86421357);
	if (argc>1) {
		htab_size = atoi(argv[1]);
	}

	/* create the empty hash table */
	H = hash_table_create(htab_size);

	while (getword(word, WORDLEN) != EOF) {
		nwords += 1;
		/* search the hash table */
		found = (hash_table_search(H, word, strcmp) != NULL);
		if (!found) {
			/* duplicate the string and insert it */
			new = malloc(strlen(word)+1);
			assert(new);
			strcpy(new, word);
			H = hash_table_insert(H, new);
			dwords += 1;
		}
	}
	
	fprintf(stderr, "Processed %d words, ", nwords);
	fprintf(stderr, "%d distinct\n", dwords);
	fprintf(stderr, "Required %d strcmps, ", tot_cmps);
	fprintf(stderr, "%.2f per word\n", 1.0*tot_cmps/nwords);
	
	/* and peer inside the hash table to gather some stats */
	min = max = H->buckets[0].bucket_size;
	mincount = 1;
	maxloc = 0;
	for (i=1; i<htab_size; i++) {
		if (H->buckets[i].bucket_size<min) {
			min = H->buckets[i].bucket_size;
			mincount = 1;
		} else if (H->buckets[i].bucket_size==min) {
			mincount += 1;
		}
		if (H->buckets[i].bucket_size>max) {
			max = H->buckets[i].bucket_size;
			maxloc = i;
		}
	}
	fprintf(stderr, "Min bucket = %d items, %d of this size\n",
			min, mincount);
	fprintf(stderr, "Max bucket = %d\n", max);
	printf("Contents of max bucket (%d)\n", maxloc);
	for (i=0; i<H->buckets[maxloc].bucket_size; i++) {
		printf("%s\n", H->buckets[maxloc].ptrs[i]);
	}

	hash_table_free(H);
	return 0;
}

/* =====================================================================
   Program written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
   ================================================================== */