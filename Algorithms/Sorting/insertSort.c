#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void insertionSort(int *A, int n);
int intCompare(const void *a, const void *b);
void genInsertSort(void *A, int nmemb, size_t size, int (*compar)(const void *, const void *));

int main(int argc, char *argv[]) {
	int A[7] = {3,1, 5, 7, 2, 9, 4};

	insertionSort(A, 7);

	for (int i = 0; i < 7; i++) {
		printf("%d", A[i]);
		if (i < 6) printf(" ");
	}
    printf("\n");


    int B[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    genInsertSort(B, 10, sizeof(int), intCompare);
	
    for(int i = 0; i < 10; i++) {
        printf("%d", B[i]);
        if (i < 9) printf(" ");
    }
    
    printf("\n");
	return 0;
}

void insertionSort(int *A, int n) {
	for (int i = 1; i < n; i++) {
		int key = A[i];
		int j = i - 1;
		while (j >= 0 && A[j] > key) {
			A[j+1] = A[j];
            j--;
		}
		A[j+1] = key;
	}
}

int intCompare(const void *a, const void *b) {
    const int* A = (const int *)a;
    const int* B = (const int *)b;
    return (*A > *B) - (*A < *B);
}

void genInsertSort(void *A, int nmemb, size_t size, int (*compar)(const void *, const void *)) {
    char *arr = (char *)A;
    for (int i = 1; i < nmemb; i++) {
        // char *key = *(arr + i * size); // We can't do this since we don't know
        // the values of the elements in A in the first place. Which is why a pointer is needed to store the key.
        char *key = malloc(size);
        memcpy(key, arr + i * size, size);

        int j = i - 1;
        while (j >= 0 && compar(arr + j * size, key) > 0) {
            memcpy(arr + (j + 1) * size, arr + j * size, size);
            j--;
        }

        memcpy(arr + (j + 1)*size, key, size);
        free(key);
    }
}