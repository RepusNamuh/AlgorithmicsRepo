/* A KMP string matching implementation*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int getString(char *str, int length, int pattern);
void shiftArray(int *array, char *pattern, int size);
void BMP(char *input, int len, char *pattern, int size);

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

    BMP(input, len, pattern, size);

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

void shiftArray(int *array, char *pattern, int size) {
    for (int i = 0; i < 256; i++) {
        array[i] = size; // default shift value
    }
    for (int i = 0; i < size - 1; i++) {
        array[(int)pattern[i]] = size - 1 - i;
    }
}

void BMP(char *input, int len, char *pattern, int size) {
    int shift[256];
    shiftArray(shift, pattern, size);

    int i = 0; // index for input
    int j = size - 1;     // index for pattern
    int flag = 0;
    
    while (i <= len - size) {
        if (input[i + j] != pattern[j]) {
            i += shift[(int)input[i + size - 1]];
            j = size - 1;
        }
        else if (j == 0) {
            printf("Pattern found at index %d\n", i);
            flag = 1;
            i += shift[(int)input[i + size - 1]];
            j = size - 1;
        }
        else {
            j--;
        }
    }
    if (!flag) {
        printf("Pattern not found\n");
    }

}