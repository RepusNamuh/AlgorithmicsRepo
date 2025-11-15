#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 10000

void mergeSort(int currentArray[], int start, int end);
void merger(int mergedArray[], int leftstart, int leftend, int rightstart, int rightend);
int reader(int A[], int limit);
void printArray(int A[], int size);

int main(int argc, char *argv[]) {
    int A[MAX_SIZE];
    int size = reader(A, MAX_SIZE);
    mergeSort(A, 0, size - 1);
    printf("TADAA!! SORTED ARRAY:\n");
    printArray(A, size);
    return 0;
}

int reader(int A[], int limit) {
    int count = 0;
    while (count < limit && scanf("%d", &A[count]) == 1) {
        count++;
    }
    return count;
}

void printArray(int A[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}


void mergeSort(int currentArray[], int start, int end) {
    if (start >= end) {
        return;
    }

    int mid = start + (end - start) / 2;

    mergeSort(currentArray, start, mid);
    mergeSort(currentArray, mid + 1, end);
    merger(currentArray, start, mid, mid + 1, end);
}

void merger(int mergedArray[], int leftstart, int leftend, int rightstart, int rightend) {
    int leftSize = leftend - leftstart + 1;
    int rightSize = rightend - rightstart + 1;  

    int *leftArray = (int *)malloc(leftSize * sizeof(int));

    for (int i = 0; i < leftSize; i++) {
        leftArray[i] = mergedArray[leftstart + i];
    }

    int i = 0, j = rightstart, k = leftstart;
    while (i < leftSize && j < rightend + 1) {
        if (leftArray[i] <= mergedArray[j]) {
            mergedArray[k++] = leftArray[i++];
        } else {
            mergedArray[k++] = mergedArray[j++];
        }
    }

    while (i < leftSize) {
        mergedArray[k++] = leftArray[i++];
    }
    while (j < rightSize) {
        mergedArray[k++] = mergedArray[j++];
    }

    free(leftArray);
}