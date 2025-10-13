/* Program that transforms a given initial two-dimensional matrix into a target
matrix by applying a sequence of matrix manipulations.
The program was written by Artem Polyvyanyy, http://polyvyanyy.com/, with the
intention to demonstrate to students an example solution to Assignment 2
of the Foundations of Algorithms (comp10002) subject (Semester 2, 2025).
All included code is (c) Copyright University of Melbourne, 2025.
Authorship Declaration:
(1) I certify that except for the code provided in the initial skeleton file,
the program contained in this submission is completely my own individual
work, except where explicitly noted by further comments that provide details
otherwise. I understand that work that has been developed by another student,
or by me in collaboration with other students, or by non-students as a result
of request, solicitation, or payment, may not be submitted for assessment in
this subject. I understand that submitting for assessment work developed by
or in collaboration with other students or non-students constitutes Academic
Misconduct, and may be penalized by mark deductions, or by other penalties
determined via the University of Melbourne Academic Honesty Policy, as
described at https://academicintegrity.unimelb.edu.au.
(2) I also certify that I have not provided a copy of this work in either
softcopy or hardcopy or any other form to any other student, and nor will I
do so until after the marks are released. I understand that providing my work
to other students, regardless of my intention or any undertakings made to me
by that other student, is also Academic Misconduct.
(3) I further understand that providing a copy of the assignment specification
to any form of code authoring or assignment tutoring service, or drawing the
attention of others to such services and code that may have been made
available via such a service, may be regarded as Student General Misconduct
(interfering with the teaching activities of the University and/or inciting
others to commit Academic Misconduct). I understand that an allegation of
Student General Misconduct may arise regardless of whether or not I personally
make use of such solutions or sought benefit from such actions.
Signed by: Cong Le
Dated: 25 September 2025
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n" // stage delimiter
#define THEEND "==THE END============================\n" // end message
#define INIT "Initial"
#define TARGET "Target"
#define CURR "Current"

#define MTXDIM "%dx%d\n" // matrix dimensions input format
#define NOTFOUND -1      // return value if search was unsuccessful
#define TRUE 1
#define FALSE 0
#define INIT_SIZE 2      // initial size of row array
#define ASCII_SIZE 256   // size of ASCII table
#define EMPTY 0          // empty cell value
#define PRINTABLE 35     // MAX matrix size
#define MAXDIGIT 9       // max digit in matrix

/* TYPE DEFINITIONS ----------------------------------------------------------*/
// Compressed Sparse Row (CSR) matrix representation

typedef struct{
    int col, val; 
} cell_t;

typedef struct {
    cell_t *row;
    int nElements, row_size;
} row_t;

typedef struct {
    int rows;       // number of rows in this matrix
    int cols;       // number of columns in this matrix
    int notMatrix;  // flag for printing matix or list
    int nnz;     // number of stored non-zeros values in this matrix
    int ended;      // flag for ended program
    row_t** rptr;   // row pointers
} CSRMatrix_t;

typedef struct {
    int r1, r2, c1, c2, val, add; // add=0 for multiplication.
} changeInfo_t;

typedef struct {
    char *format;
    void (*func)(CSRMatrix_t*, changeInfo_t*);
} change_t;


/* FUNCTION PROTOTYPES -------------------------------------------------------*/

// Matrix creation
CSRMatrix_t* csr_matrix_create(int, int); // create empty CSR matrix
row_t* create_arrayInfo(int occupied, int size);
void make_cell(cell_t * cell, int col, int val);

// memory reallocation and freeing
void csr_matrix_free(CSRMatrix_t*); // free input CSR matrix
void free_arrayInfo(row_t* array);
void upSize(row_t *array, int new_size);

// Reader functions
void clearGarbageInput();
void get_matrix(CSRMatrix_t *A);
void scanPrintInstruction(char *changeForm, char command, changeInfo_t *info);

// Insertion function set
int binarySearch(cell_t *array, int size, int target, int *ist);
void insert(row_t *array, int location, int row, int col);
void null_check(CSRMatrix_t *A, int index);
int getOrInsert(CSRMatrix_t *A, int rowval, int colval, int *index);

// Printing functions
void printRowForm(row_t *row, int end);
void printCoordinateForm(row_t *array, int row);
void printMatrix(CSRMatrix_t *A);
void printHeader(CSRMatrix_t *A, const char *label);
void matrixStatus(CSRMatrix_t *A, char *type);

// Change functions
void nonZeroCount(int prev, int curr, double *count);
void set_cell(CSRMatrix_t *A, changeInfo_t *info);
void swap_int(int *a, int *b);
void swap_cell(CSRMatrix_t *A, changeInfo_t *info);
void add_or_multi(CSRMatrix_t *A, changeInfo_t *info);
void copy_row(CSRMatrix_t *A, changeInfo_t *info);
void swap_row(CSRMatrix_t *A, changeInfo_t *info);
void swap_col(CSRMatrix_t *A, changeInfo_t *info);
void copy_col(CSRMatrix_t *A, changeInfo_t *info);
change_t* linkCommandWithFunc();

// Main set of functions for each stage
void printInitialStat(CSRMatrix_t *A, CSRMatrix_t *B);
int stageOneCommand(char command);
void commandReader(CSRMatrix_t *A,CSRMatrix_t *B, int stage, 
                                int *count, change_t *changes);
int compareMatrix(CSRMatrix_t *A, CSRMatrix_t *B);
void remove_zero(row_t *row, int idx);
void notMatrix_check(CSRMatrix_t *A);
void theEnd(int *count);


/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char *argv[]) {
    int stage=0, rows, cols, count = 0;
    printf(SDELIM, stage++); // print Stage 0 header
    scanf(MTXDIM, &rows, &cols); // assert matrix dimensions
    // create initial matrix of 0s
    CSRMatrix_t* inputMatrix = csr_matrix_create(rows,cols);
    // create target matrix of 0s
    CSRMatrix_t* targetMatrix= csr_matrix_create(rows,cols);
    // ...
    
    // Read the input 
    get_matrix(inputMatrix); // read initial matrix
    get_matrix(targetMatrix); // read target matrix

    notMatrix_check(inputMatrix);  // bulk check of out of bound value
    notMatrix_check(targetMatrix);

    printInitialStat(inputMatrix, targetMatrix);
    change_t *changes = linkCommandWithFunc();

    clearGarbageInput();
    // Early termination check
    inputMatrix->ended = compareMatrix(inputMatrix, targetMatrix);

    while (!inputMatrix->ended && stage <= 2) {
        printf(SDELIM, stage); // print Stage header
        commandReader(inputMatrix, targetMatrix, stage++, &count, changes);
    }
    
    if (inputMatrix->ended) {
        theEnd(&count); 
    }
    printf(THEEND); // print "THE END" message

    csr_matrix_free(inputMatrix); // free initial matrix
    csr_matrix_free(targetMatrix); // free target matrix
    free(changes);
    return EXIT_SUCCESS; // algorithms are fun!!!
}

/****************************************************************************/
/************** MEMORY ALLOCATIONS AND FREEING FUNCTIONS********************/
/****************************************************************************/

// Create an empty CSR matrix of nrows rows and ncols columns
CSRMatrix_t *csr_matrix_create(int nrows, int ncols) {
    // allocate memory for this matrix
    assert(nrows >= 0 && ncols >= 0); // check matrix dimensions
    
    CSRMatrix_t *A = (CSRMatrix_t*)malloc(sizeof(CSRMatrix_t));
    assert(A!=NULL); // check if memory was allocated
    A->rows = nrows; // set number of rows in the matrix
    A->cols = ncols; // set number of columns in the matrix
    A->nnz = EMPTY; // initialize with no non-zero values
    A->notMatrix = A->ended = FALSE;
    // allocate array to store row pointers
    A->rptr = (row_t**)malloc((size_t)(A->rows+1)*sizeof(row_t*));
    assert(A->rptr!=NULL);
    for (int i = 0; i <= A->rows; i++) { // no values, so initialize ...
        A->rptr[i] = NULL;
    }
    return A;
}

void csr_matrix_free(CSRMatrix_t *A) {
    assert(A!=NULL);
    for (int i = 0; i <= A->rows; i++) {
        free_arrayInfo(A->rptr[i]); // free each row
    }
    free(A->rptr);
    free(A); // free the matrix structure itself
}

void free_arrayInfo(row_t *array) {
    if (array == NULL) return;
    free(array->row);
    free(array);
}
// Generate an empty list
row_t* create_arrayInfo(int occupied, int size) {
    row_t *list = (row_t*)malloc(sizeof(row_t));
    assert(list != NULL);

    list->nElements = occupied;
    list->row_size = size;  
    list->row = (cell_t*)malloc(sizeof(cell_t) * size);
    assert(list->row != NULL);

    return list;
}

void upSize(row_t *array, int new_size) {
    cell_t *new = (cell_t*)realloc(array->row, sizeof(cell_t) * new_size);
    assert(new != NULL);
    
    array->row_size = new_size;
    array->row = new;
}

void make_cell(cell_t * cell, int col, int val) {
    cell->col = col, cell->val = val;
}

void remove_zero(row_t *row, int idx) {
    // Shift the memory to the left.
    memmove(&row->row[idx], &row->row[idx + 1], 
            sizeof(cell_t) * (row->nElements - idx - 1));
    row->nElements--;
}

// Checking if the row_t pointer is NULL
void null_check(CSRMatrix_t *A, int index) {
    if (A->rptr[index] == NULL) {
        A->rptr[index] = create_arrayInfo(EMPTY, INIT_SIZE);
    }
}

/****************************************************************************/
/*************************SEARCHING AND INSERTIONS***************************/
/****************************************************************************/

// Searching for both exact match and insertion point 
// *ist is the point of insertion, can be pass as NULL
int binarySearch(cell_t *array, int size, int target, int *ist) {
    int left = 0;
    int right = size - 1;
    while (left <= right && array != NULL) {
        int mid = (right + left) / 2;
        if (array[mid].col == target) {  
            if (ist) *ist = mid;  // exact match found
            return mid;
        }
        else if (array[mid].col < target) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }
    if (ist) {
        *ist = left; // point of insertion
    }
    return NOTFOUND;
}

void insert(row_t *array, int location, int row, int col) {
    // If the array is full, we need to resize it.
    if(array->nElements + 1 > array->row_size) {
        upSize(array, array->row_size * 2);
    }
    // Shift every element from location to the right.
    if (location < array->nElements) {
        memmove(&array->row[location + 1], &array->row[location], 
                sizeof(cell_t) * (array->nElements - location));
    }
    // initiate the new cell with val of 0
    make_cell(&array->row[location], col, EMPTY);
    array->nElements++;
}

int getOrInsert(CSRMatrix_t *A, int rowval, int colval, int *index) {
    null_check(A, rowval);
    row_t *arrInfo = A->rptr[rowval];

    int insertPos;
    // We either find an exact cell or it is NOTFOUND.
    int pos = binarySearch(arrInfo->row, arrInfo->nElements, 
                                                colval, &insertPos);
    if (pos != NOTFOUND) {
        // We found the cell; 
        *index = pos;
        return FALSE;
    }
    // We did not find the cell but the insert index, so we insert.
    insert(arrInfo, insertPos, rowval, colval);
    *index = insertPos;
    return TRUE;
}

/************************************************************************/
/**************************READER FUNCTIONS******************************/
/************************************************************************/

void clearGarbageInput() {
    int c;
    while ((c = getchar()) != EOF && 
           !((c >= '0' && c <= '9') || c == '-' || isalpha(c))) {
        // Skip non-digit characters and non-minus characters
    }
    ungetc(c, stdin);
}

void get_matrix(CSRMatrix_t *A) {
    int val, row, col;
    row_t *arrInfo = NULL;
    clearGarbageInput();

    while (scanf("%d,%d,%d", &row, &col, &val) == 3) {

        null_check(A, row);  // create row if it does not exist
        arrInfo = A->rptr[row];

        int index = NOTFOUND;
        getOrInsert(A, row, col, &index);
        make_cell(&arrInfo->row[index], col, val);
    }
}

// Creating an array of change format and 
// function pointer for each command
change_t *linkCommandWithFunc() {
    change_t *table = (change_t*)calloc(ASCII_SIZE, sizeof(change_t));
    assert(table != NULL);

    table['s'].format = ":%d,%d,%d\n";    table['s'].func = set_cell;
    table['S'].format = ":%d,%d,%d,%d\n"; table['S'].func = swap_cell;
    table['m'].format = ":%d\n";          table['m'].func = add_or_multi;
    table['a'].format = ":%d\n";          table['a'].func = add_or_multi;
    table['R'].format = ":%d,%d\n";       table['R'].func = swap_row;
    table['C'].format = ":%d,%d\n";       table['C'].func = swap_col;
    table['r'].format = ":%d,%d\n";       table['r'].func = copy_row;
    table['c'].format = ":%d,%d\n";       table['c'].func = copy_col;

    return table;
}

// A function that scan and print base on the command given.
void scanPrintInstruction(char *changeForm, char command, changeInfo_t *info) {
    printf("INSTRUCTION %c", command);
    if (command == 's') {
        scanf(changeForm, &info->r1, &info->c1, &info->val);
        printf(changeForm, info->r1, info->c1, info->val);
    }
    else if (command == 'S') {
        scanf(changeForm, &info->r1, &info->c1, &info->r2, &info->c2);
        printf(changeForm, info->r1, info->c1, info->r2, info->c2);
    }
    else if (command == 'm' || command == 'a') {
        scanf(changeForm, &info->val);
        printf(changeForm, info->val);
    }
    else if (command == 'R' || command == 'r' ) {
        scanf(changeForm, &info->r1, &info->r2);
        printf(changeForm, info->r1, info->r2);
    }
    else {
        scanf(changeForm, &info->c1, &info->c2);
        printf(changeForm, info->c1, info->c2);
    }
}

/****************************************************************************/
/************************** PRINTING FUNCTIONS ******************************/
/****************************************************************************/

void printRowForm(row_t *array, int end) {
    int start = 0;

    printf("[");
    if (array != NULL) {    
        for (int i = 0; i < array->nElements; i++) {
            int col = array->row[i].col;
            int val = array->row[i].val;

            // Skipping zero values
            if (val != EMPTY) {
                // Print spaces from the last non-zero
                printf("%*s%d", col - start, "", val);
                start = col + 1;
            }
        }
    }
    // print remaining spaces till the end of the row
    printf("%*s]\n", end - start, "");
}

void printCoordinateForm(row_t *array, int row) {
    if (array == NULL) return;
    for (int i = 0; i < array->nElements; i++) {
        int col = array->row[i].col;
        int val = array->row[i].val;
        
        // Skip zero values
        if (val != EMPTY) {
            printf("(%d,%d)=%d\n", row, col, val);
        }
    }
}

void printMatrix(CSRMatrix_t *A) {
    for (int i = 0; i < A->rows; i++) {
        row_t *arrInfo = A->rptr[i];

        // If matrix is too big or not in range.
        if (A->cols > PRINTABLE || A->rows > PRINTABLE || A->notMatrix) {
            printCoordinateForm(arrInfo, i);
        } 
        else {
            printRowForm(arrInfo, A->cols);
        }
    }
}

void printHeader(CSRMatrix_t *A, const char *label) {
    printf("%s matrix: %dx%d, nnz=%d\n", label, 
                                A->rows, A->cols, A->nnz);
}

void matrixStatus(CSRMatrix_t *A, char *type) {
    printHeader(A, type);
    printMatrix(A);
}

/****************************************************************************/
/************************** CHANGE FUNCTIONS ********************************/
/****************************************************************************/

void swap_int(int*a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swap_cell(CSRMatrix_t *A, changeInfo_t *info) {
    // if both rows are NULL, nothing to swap
    if (A->rptr[info->r1] == NULL 
        && A->rptr[info->r2] == NULL) return;
        
    // Get the cell with the target column, insert if not found
    int i1 = NOTFOUND, i2 = NOTFOUND;
    getOrInsert(A, info->r1, info->c1, &i1);
    int inserted = getOrInsert(A, info->r2, info->c2, &i2);

    // If row1 == row2 and we inserted at i2 a new cell when i2 < i1
    if (inserted && i1 >= i2 && info->r1 == info->r2) i1++;

    row_t *arrInfo1 = A->rptr[info->r1];
    row_t *arrInfo2 = A->rptr[info->r2];
    

    swap_int(&arrInfo1->row[i1].val, &arrInfo2->row[i2].val);
}

void set_cell(CSRMatrix_t *A, changeInfo_t *info) {
    if (info->val == 0 && A->rptr[info->r1] == NULL) return;

    int index = NOTFOUND;
    
    // Get the cell with target col, insert if not found
    getOrInsert(A, info->r1, info->c1, &index);

    A->rptr[info->r1]->row[index].val = info->val;
}

void add_or_multi(CSRMatrix_t *A, changeInfo_t *info) {
    for (int i = 0; i < A->rows; i++) {
        if (A->rptr[i] == NULL) continue;
        row_t *arrInfo = A->rptr[i];

        for (int j = 0; j < arrInfo->nElements; j++) {
            int *curr = &arrInfo->row[j].val;
            // We don't add if empty or if command is m.
            if (info->add && *curr != EMPTY) {
                *curr += info->val;
            } else {
                *curr *= info->val;
            }
        }
    }
}

// Swap the row via pointer swapping
void swap_row(CSRMatrix_t *A, changeInfo_t *info) {
    row_t *temp = A->rptr[info->r1];
    A->rptr[info->r1] = A->rptr[info->r2];
    A->rptr[info->r2] = temp;
}

void copy_row(CSRMatrix_t *A, changeInfo_t *info) {
    if (info->r1 == info->r2) return; // copying to itself, do nothing
    row_t *src = A->rptr[info->r1];
    row_t *target = A->rptr[info->r2];
    free_arrayInfo(target); // free the target row first

    if (src == NULL) {
        A->rptr[info->r2] = NULL;
        return;
    }
    // Create a new row_t with everything except values in row
    target = create_arrayInfo(src->nElements, src->row_size);
    // Copy everything from src to target
    memcpy(target->row, src->row, sizeof(cell_t) * src->nElements);

    A->rptr[info->r2] = target;
} 

// Go through each row, and swap the cell in that row.
void swap_col(CSRMatrix_t *A, changeInfo_t *info) {
    for (int i = 0; i < A->rows; i++) {
        info->r1 = i;
        info->r2 = i;
        swap_cell(A, info);
    }
}

void copy_col(CSRMatrix_t*A, changeInfo_t *info) {
    // src and target is c1 and c2, swap them for set_cell manipulation
    swap_int(&info->c1, &info->c2);

    for (int i = 0; i < A->rows; i++) {
        row_t *row = A->rptr[i];
        int i1 = NOTFOUND;
        info->r1 = i;

        // If row exist, search for the source cell,
        if (row) {
            // which is at c2, if cell is found, set the 
            i1 = binarySearch(row->row, row->nElements, info->c2, NULL);
            info->val = (i1 != NOTFOUND)? row->row[i1].val : EMPTY;
            
            // value of that cell to c1 otherwise set c1 cell to 0.
            set_cell(A, info);
        }
    }
}

/************************************************************************/
/*******************MAIN SET OF FUNCTIONS FOR EACH STAGE*****************/
/************************************************************************/

void printInitialStat(CSRMatrix_t *A, CSRMatrix_t *B) {
    matrixStatus(A, INIT); // print initial matrix

    printf("-------------------------------------\n");

    matrixStatus(B, TARGET); // print target matrix
}

// Check if the command is valid for stage 1
int stageOneCommand(char cmd) {
    return (cmd == 's' || cmd == 'S' || cmd == 'm' || cmd == 'a');
}

// A function that perform both stage 1 and 2.
//Read the command. IF we are still in STAGE1 and 
//command is of STAGE2, break the current reading.
//Scanf the changeform base on the command.
//Call the function pointer to perform the change.
// Print out the matrix after manipulation.
void commandReader(CSRMatrix_t *A, CSRMatrix_t *B, int stage, 
                                int *count, change_t *changes) {
    char command;
    changeInfo_t info;

    while (scanf("%c", &command) == 1) {

        if (stage == 1 && !stageOneCommand(command)) {
            ungetc(command, stdin);
            break;
        }

        char *changeForm = changes[(int)command].format;
        void (*func)(CSRMatrix_t*, changeInfo_t*) 
                            = changes[(int)command].func;
        //Decide Whether it is addition or multiplication
        // for add_or_multi function    
        info.add = (command == 'a') ? TRUE : FALSE;

        // Get the change information
        scanPrintInstruction(changeForm, command, &info);

        func(A, &info);
        // Bulk check of out of bound value in Matrix
        notMatrix_check(A);

        matrixStatus(A, CURR);
        matrixStatus(B, TARGET);
        (*count)++;

        // Check if we have reached the target matrix
        if (compareMatrix(A, B)) {
            A->ended = TRUE;
            break;
        }
    }
}

void theEnd(int *count) {
    printf("-------------------------------------\n");
    printf("TA-DAA!!! SOLVED IN %d STEP(S)!\n", *count);
}

// Return TRUE if A and B are the same matrix
int compareMatrix(CSRMatrix_t *A, CSRMatrix_t *B) {
    if (A->nnz != B->nnz) {
        return FALSE;
    }

    for (int i = 0; i < A->rows; i++) {
        row_t *rowA = A->rptr[i];
        row_t *rowB = B->rptr[i];
        
        if (rowA == NULL && rowB == NULL) continue;
        if (rowA == NULL && rowB != NULL) return FALSE;

        // Row B is empty mean rowA must also be empty
        if (!rowB) {
            for (int j = 0; j < rowA->nElements; j++) {
                if (rowA->row[j].val) {
                    return FALSE; // rowA has non-zero value
                }
            }
            continue;
        }
        // Search through each cell in rowB and find it in rowA
        // There might be cell in A that is not in B, but that doesn't 
        // matter, since there will be a row that have less cell in A than B
        // guarantee by A->nnz = B->nnz
        for (int j = 0; j < rowB->nElements; j++) {
            int index = binarySearch(rowA->row, 
                    rowA->nElements, rowB->row[j].col, NULL);
            if (index == NOTFOUND) {
                return FALSE;  // column in rowB not found in rowA
            }
            if (rowA->row[index].val != rowB->row[j].val) {
                return FALSE;  // values do not match
            }
        }
    }
    return TRUE;
}

// Bulk check since this is sparse matrix and it is easier 
// to implement this way.
void notMatrix_check(CSRMatrix_t *A) {
    A->nnz = 0;
    A->notMatrix = FALSE;

    for (int i = 0; i < A->rows; i++) {
        row_t *row = A->rptr[i];
        if (row == NULL) continue;

        for (int j = 0; j < row->nElements; j++) {
            int val = row->row[j].val;

            // Out of bound values
            if (val < EMPTY || val> MAXDIGIT) {
                A->notMatrix = TRUE;
            }
            // Count non-zero values
            if (val != EMPTY) {
                A->nnz++;
            } else {
                remove_zero(row, j);
                j--;
            }

        }
    }
}
