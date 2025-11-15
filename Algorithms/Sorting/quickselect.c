/* Exercise 12.08 Median Quickselect
 * Alistair Moffat, October 2013
 * Maria Bulychev, August 2025
 * (c) University of Melbourne */

#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 1000

int quickselect(int A[], int left, int right, int k);
void swap(int *n1, int *n2);
int partition(int A[], int left, int right);

int main(int argc, char *argv[]){
    int A[MAX_SIZE], n=0, x;

    printf("Enter as many as 1000 values, ^D to end\n");
    while (n < MAX_SIZE && scanf("%d", &x) == 1) {
        A[n++] = x;
    }

    if (n == 0) {
        printf("No numbers entered.\n");
        return 1;
    }

    double median;
    if (n % 2 == 1) {               // even number of entries
        median = quickselect(A, 0, n - 1, n / 2);
    } else {                        // odd number of entries
        int m1 = quickselect(A, 0, n - 1, n / 2 - 1);
        int m2 = quickselect(A, 0, n - 1, n / 2);
        median = (m1 + m2) / 2.0;
    }

    printf("Median: %.2f\n", median);
    return 0;
}

/* exchange the values of the two variables indicated
 * by the arguments (insertionsort.c, Figure 7.4 PPSAA) */
void swap(int *n1, int *n2) {
    int temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}
int quickselect(int A[], int left, int right, int k){
    if (left == right) return A[left];

    while (left <= right) {
        int pivotIndex = partition(A, left, right);
        if (k == pivotIndex) {
            return A[k];
        } else if (k < pivotIndex) {
            right = pivotIndex - 1;
        } else {
            left = pivotIndex + 1;
        }
    }
    return A[left]; // fallback (shouldn't happen)
}

int partition(int A[], int left, int right) {
    // Lomuto partition: pivot -> A[right]
    int pivotIndex = left + (right - left) / 2;
    int pivot = A[pivotIndex];
    swap(&A[pivotIndex], &A[right]);
    int store = left;
    for (int i = left; i < right; i++) {
        if (A[i] < pivot) {
            swap(&A[i], &A[store]);
            store++;
        }
    }
    swap(&A[store], &A[right]);
    return store;
}
/* your functions */