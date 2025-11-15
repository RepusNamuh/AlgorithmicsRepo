#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SIZE 10000

int reader(int A[], int limit);
void heapsort(int A[], int size);
void printArray(int A[], int size);
void swap(int *a, int *b);
int leftChild(int index);
int rightChild(int index);
void maxheap(int A[], int size);

int main(int argc, char *argv[]) {
    int A[MAX_SIZE];
    int size = reader(A, MAX_SIZE);
    heapsort(A, size);
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

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int leftChild(int index) {
    return 2 * index + 1;
}

void siftdown(int A[], int node, int size) {
    while (1) {
        int left = leftChild(node);
        int right = left + 1;
        int largest = node;

        if (left < size && A[left] > A[largest]) {
            largest = left;
        }
        if (right < size && A[right] > A[largest]) {
            largest = right;
        }
        if (largest != node) {
            swap(&A[node], &A[largest]);
            node = largest;
        } else {
            break;
        }
    }
}
// Generate the initial max heap from the array
void maxheap(int A[], int size) {
    for (int i = (size/2) - 1; i >= 0; i--) {
        siftdown(A, i, size);
    }
}
void heapsort(int A[], int size) {
    maxheap(A, size);
    swap(&A[0], &A[--size]);

    while (size >= 1) {
        siftdown(A, 0, size);
        swap(&A[0], &A[--size]);
    }
}