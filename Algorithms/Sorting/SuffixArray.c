#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 100000000
char S[LIMIT];
int m=25;

void partition(char *string, int suffixArray[], int start, int end, int *i, int *j, int depth);
void ternaryQSort(char *string, int *suffixArray, int start, int end, int depth);

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
int
read_sequence(char *S, int limit) {
	int n=0;
	int next;
	while ((next=getchar()) != EOF) {
		if (n==limit) {
			fprintf(stderr, "Too much data, limit is %d\n",
				limit);
			exit(EXIT_FAILURE);
		}
		S[n++] = next;
	}
	S[n] = '\0';
	return n;
}

int
main(int argc, char **argv) {
	int n = read_sequence(S, LIMIT);
	if (argc>1) {
		m = atoi(argv[1]);
	}
	fprintf(stderr, "n = %d = %.1e\n", n, (double)n);
	fprintf(stderr, "m = %d\n", m);

	/* TODO: Write your implementation */
	int suffixArray[n];
	for (int i = 0; i < n; i++) {
		suffixArray[i] = i;
	}
	
	ternaryQSort(S, suffixArray, 0, n-1, 0);

	for (int i = 0; i < n; i++) {
		printf("%2d: %s", suffixArray[i], S + suffixArray[i]);
	}

	return 0;
}

void partition(char *string, int suffixArray[], int start, int end, int *i, int *j, int depth) {
	int mid = start + (end - start) / 2;
	swap(&suffixArray[start], &suffixArray[mid]); // Move pivot to start
	char pivot = string[suffixArray[mid] + depth]; 

	*i = start;
	*j = end;
	int k = start;

	while (k <= *j) {
		char current = string[suffixArray[k] + depth];
		// Less than pivot, swap the current element with the element at i.
		// We then increment i by one to increase the size of "less the pivot"
		// We also increment k to move to the next character
		if (current < pivot) {
			swap(&suffixArray[k++], &suffixArray[(*i)++]);
		}
		else if (current > pivot) {
			swap(&suffixArray[k], &suffixArray[(*j)--]);
		}
		// Equal to pivot? Read the next character
		else {
			k++;
		}
	}
}

void ternaryQSort(char *string, int *suffixArray, int start, int end, int depth) {
	if (start >= end) {
		return;
	}
	int i, j;

	partition(string, suffixArray, start, end, &i, &j, depth);

	ternaryQSort(string, suffixArray, start, i - 1, depth);
	if (string[suffixArray[i] + depth] != '\0') {  // Only sort equal elements if not at end of string
		ternaryQSort(string, suffixArray, i, j, depth + 1);
	}
	ternaryQSort(string, suffixArray, j + 1, end, depth);
}