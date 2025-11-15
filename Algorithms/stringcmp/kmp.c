/* A KMP string matching implementation*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int getString(char *str, int length, int pattern);
void failureArray(char *pattern, int size, int *fArray);
void KMP(char *input, int len, char *pattern, int size);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        argv[1] = "1000";
        argv[2] = "10";
    }
    char *input = malloc(atoi(argv[1]) * sizeof(char));
    char *pattern = malloc(atoi(argv[2]) * sizeof(char));

    // Read input
    printf("length1, length2: %d %d\n", atoi(argv[1]), atoi(argv[2]));

    printf("Enter the input string: "); 
    int len = getString(input, atoi(argv[1]), 0);
    
    printf("Enter the pattern to search: ");
    int size = getString(pattern, atoi(argv[2]), 1);

    fprintf(stdout, "Input String of length %d", len);
    printf("\n");
    fprintf(stdout, "Pattern of length %d", size);

    KMP(input, len, pattern, size);

    return 0;
}

int getString(char *str, int length, int pattern) {
    char c;
    int count = 0;
    int excess = 0;
    
    // Read chacters until end of file or newline for pattern input
    while ((c = getchar()) != EOF) {
        if (pattern && c == '\n') {
            break;
        }
        if (count >= length) {
            excess++;
            continue;
        }
        str[count++] = c;
    }
    str[count] = '\0';
    return count;
}

// Start of KMP algorithm implementation
// Think of fArray as a way to skip unnecessary comparisons
//  1 2 3 4 5 6 7 8 9 10           // Indices
//  A B A B A C A B A D            // Pattern
// -1 0 0 1 2 0 1 2 3 0            // Failure Array
// 2 means that the character at index j - 1 matches the character at index 2.
// That's why there is a j = fArray[j] in the code.
// So when a mismatch happens at index j, we can skip straight to the last
// known longest prefix suffix. 
void failureArray(char *pattern, int size, int *fArray) {
    fArray[0] = -1;
    fArray[1] = 0;
    int i = 2;
    int j = 0;

    while (i < size) {
        char end = pattern[i - 1];
        char start = pattern[j];
        if (end == start) {
            fArray[i] = ++j;
            i++;
        }
        else if (j > 0) {
            j = fArray[j];
        }
        else {
            fArray[i] = 0;
            i++;
        }
    }
}

void KMP(char *input, int len, char *pattern, int size) {
    int flag = 0;
    
    int *fArray = malloc(size * sizeof(int));
    failureArray(pattern, size, fArray);
    printf("\nFailure Array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", fArray[i]);
    }
    printf("\n");
    
    int i = 0; // index for input
    int j = 0; // index for pattern

    while (i < len - size) {
        if (input[i + j] == pattern[j]) {
            if (j + 1 == size) {
                printf("Pattern found at index %d\n", i);
                // Move i all the way to next i + j first.
                // Then moveback by an amount equal to the size of fArray[j]
                // Which is the size of the longest prefix suffix of the pattern.
                i = i + j - fArray[j];
                j = 0;
                flag = 1;
            }
            j++;
        }
        else {
            // Move i all the way to next i + j first.
            // Then moveback by an amount equal to the size of fArray[j]
            // Which is the size of the longest prefix suffix of the sub-pattern.
            i = i + j - fArray[j];
            j = (j > 0) ? fArray[j] : 0;
        }
    }
    if (!flag) {
        printf("Pattern not found in the input string.\n");
    }
}