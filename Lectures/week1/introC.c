#include <stdio.h>


#define FINALCHAR 128
int main(int argc, char *argv[]) {
    printf("           +0  +1  +2  +3  +4  +5  +6  +7\n");
    printf("        +--------------------------------\n");

    int currenChar = 32;
    while (currenChar < FINALCHAR) {
        printf("     %3d |   %c   %c   %c   %c   %c   %c   %c   %c\n", 
                currenChar, currenChar + 1, currenChar + 2, 
                currenChar + 3, currenChar + 4, currenChar + 5,
                currenChar + 6, currenChar + 7);
        currenChar += 8;
    }
}