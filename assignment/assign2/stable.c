/* Program that transforms a given initial two-dimensional matrix into a target
matrix by applying a sequence of matrix manipulations.
The skeleton program was written by Artem Polyvyanyy, http://polyvyanyy.com/, 
with the intention to demonstrate to students an example solution to Assignment 
2 of the Foundations of Algorithms (comp10002) subject (Semester 2, 2025).
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
    int nElements, row_size, nnz, nonMatrix;
} row_t;

typedef struct {
    int rows;       // number of rows in this matrix
    int cols;       // number of columns in this matrix
    int notMatrix;  // flag for printing matix or list
    int nnz;     // number of stored non-zeros values in this matrix
    row_t** rptr;   // row pointers
} CSRMatrix_t;

typedef struct {
    int r1, r2, c1, c2, val, add; // add=0 for multiplication.
} changeInfo_t;

typedef struct {
    char *format;
    void (*func)(CSRMatrix_t*, changeInfo_t*);
} change_t;

typedef struct {
    int foundAt, appendAt;
} search_t;


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

int colcmp(const void *a, const void *b);
void firstsort(CSRMatrix_t *A);

// Insertion function set
int binarySearch(row_t *arrInfo, int target, int *ist);
void append(row_t *array, int location, int row, int col);
int move(row_t *array, int from, int to);
void null_check(CSRMatrix_t *A, int index);
search_t getOrApend(CSRMatrix_t *A, int rowval, int colval);

// Printing functions
void printProperRow(row_t *row, int end);
void printCoordinate(row_t *array, int row);
void printMatrix(CSRMatrix_t *A);
void printHeader(CSRMatrix_t *A, const char *label);
void matrixStatus(CSRMatrix_t *A, char *type);

// Change functions
void nonZeroCount(int prev, int curr, int *mcount, int *ncount);
void notMatrix(int prev, int curr, int *mcount, int *ncount);
void set_cell(CSRMatrix_t *A, changeInfo_t *info);
void swap(int *a, int *b);
void swap_cell(CSRMatrix_t *A, changeInfo_t *info);
void add_or_multi(CSRMatrix_t *A, changeInfo_t *info);
void copy_row(CSRMatrix_t *A, changeInfo_t *info);
void swap_row(CSRMatrix_t *A, changeInfo_t *info);
void swap_col(CSRMatrix_t *A, changeInfo_t *info);
void copy_col(CSRMatrix_t *A, changeInfo_t *info);
change_t* assignChange();

// Main set of functions for each stage
void stage0(CSRMatrix_t *A, CSRMatrix_t *B);
int stage1Change(char command);
void changeReader(CSRMatrix_t *A,CSRMatrix_t *B, int stage, 
                                int *count, change_t *changes);
int compareMatrix(CSRMatrix_t *A, CSRMatrix_t *B);
int remove_zero(row_t *row, int idx);
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

    stage0(inputMatrix, targetMatrix);
    change_t *changes = assignChange();

    printf(SDELIM, stage); // print Stage 1 header
    clearGarbageInput();
    changeReader(inputMatrix, targetMatrix, stage++, &count, changes);

    if (count != NOTFOUND) {// only if not solved in stage 1
        printf(SDELIM, stage); // print Stage 2 header
        changeReader(inputMatrix, targetMatrix, stage++, &count, changes);
    }

    printf(THEEND); // print "THE END" message

    csr_matrix_free(inputMatrix); // free initial matrix
    csr_matrix_free(targetMatrix); // free target matrix
    free(changes);
    return EXIT_SUCCESS; // algorithms are fun!!!
}

// Create an empty CSR matrix of nrows rows and ncols columns
CSRMatrix_t *csr_matrix_create(int nrows, int ncols) {
    // allocate memory for this matrix
    assert(nrows >= 0 && ncols >= 0); // check matrix dimensions
    
    
    CSRMatrix_t *A = (CSRMatrix_t*)malloc(sizeof(CSRMatrix_t));
    assert(A!=NULL); // check if memory was allocated
    A->rows = nrows; // set number of rows in the matrix
    A->cols = ncols; // set number of columns in the matrix
    A->nnz = 0; // initialize with no non-zero values
    A->notMatrix = EMPTY;
    // allocate array to store row pointers
    A->rptr = (row_t**)malloc((size_t)(A->rows+1)*sizeof(row_t*));
    assert(A->rptr!=NULL);
    for (int i = 0; i <= A->rows; i++) { // no values, so initialize ...
        A->rptr[i] = NULL;
    }
    return A;
}
// Free input CSR matrix A
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
    list->nnz = list->nonMatrix = EMPTY;
    list->row_size = size;  
    list->row = (cell_t*)malloc(sizeof(cell_t) * size);
    assert(list->row != NULL);

    return list;
}

void upSize(row_t *array, int new_size) {
    if (array->nElements < array->row_size -1 ) return;
    
    cell_t *new = (cell_t*)realloc(array->row, sizeof(cell_t) * new_size);
    assert(new != NULL);
    
    array->row_size = new_size;
    array->row = new;
}

void make_cell(cell_t * cell, int col, int val) {
    cell->col = col, cell->val = val;
}

// A comparison function for first sort.
int colcmp(const void *a, const void *b) {
    cell_t *cellA = (cell_t*)a;
    cell_t *cellB = (cell_t*)b;

    if (cellA->col < cellB->col) {
        return -1;
    } else {
        return (cellA->col > cellB->col);
    }
}

void clearGarbageInput() {
    int c;
    while ((c = getchar()) != EOF && 
           !((c >= '0' && c <= '9') || c == '-' || isalpha(c))) {
        // Skip non-digit characters and non-minus characters
    }
    ungetc(c, stdin);
}
// Update non-matrix count
void notMatrix(int prev, int curr, int *mcount, int *ncount) {
    int decrement = (prev < EMPTY || prev > MAXDIGIT) ? -1 : 0;
    int increment = (curr < EMPTY || curr > MAXDIGIT) ? 1 : 0;
    *mcount += (increment + decrement);
    *ncount += (increment + decrement);
}
// Update non-zero count for matrix and row
void nonZeroCount(int prev, int curr, int *mcount, int *ncount) {
    int decrement = (prev != EMPTY) ? -1 : 0;
    int increment = (curr != EMPTY) ? 1 : 0;
    *mcount += (decrement + increment);
    *ncount += (increment + decrement);
}

void get_matrix(CSRMatrix_t *A) {
    int val, row, col;
    row_t *arrInfo = NULL;
    clearGarbageInput();

    while (scanf("%d,%d,%d", &row, &col, &val) == 3) {
        if (val == EMPTY) continue; 
        null_check(A, row);  // create row if it does not exist
        arrInfo = A->rptr[row];

        int *index = &arrInfo->nElements;
        upSize(arrInfo, *index * 2);  // increase size if bottleneck
        make_cell(&arrInfo->row[(*index)++], col, val);

        A->nnz++;
        arrInfo->nnz++; // increase non-zero count for this row
        arrInfo->nonMatrix += (val < EMPTY || val > MAXDIGIT);
        A->notMatrix += (val < EMPTY || val > MAXDIGIT);
    }
}
// Searching for both exact match and insertion point 
// *ist is the point of insertion, can be pass as NULL
int binarySearch(row_t* arrInfo, int target, int *ist) {
    if (arrInfo->row == NULL) return NOTFOUND;
    int left = 0;
    int right = arrInfo->nElements - 1;
    while (left <= right && arrInfo->row != NULL) {
        int mid = (right + left) / 2;
        if (arrInfo->row[mid].col == target) {
            if (ist) *ist = mid;
            return mid;
        }
        else if (arrInfo->row[mid].col < target) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }
    if (ist) *ist = left;
    return NOTFOUND;
}
// Add an empty cell at the end of the row
void append(row_t *array, int location, int row, int col) {
    upSize(array, array->row_size * 2);
    make_cell(&array->row[location], col, EMPTY);
    array->nElements++;
}

// insert the cell at "from" to location "to".
int move(row_t *array, int from, int to) {
    if (from == to) return FALSE; // no need to move
    if (array->row[from].val == EMPTY) {
        return remove_zero(array, from);  // remove zero value
    }
    cell_t temp = array->row[from];
    memmove(&array->row[to + 1], &array->row[to], 
            sizeof(cell_t) * (from - to));
    array->row[to] = temp;
    return FALSE;
}

int remove_zero(row_t *row, int idx) {
    if(row == NULL) {
        return FALSE;
    }
    
    if (row->row[idx].val != EMPTY) {
        return FALSE;
    }
    memmove(&row->row[idx],
            &row->row[idx + 1],
            sizeof(cell_t) * (row->nElements - idx - 1));
    row->nElements--;
    return TRUE;
}

void printProperRow(row_t *array, int end) {
    int start = 0;
    printf("[");
    if (array != NULL) {    
        for (int i = 0; i < array->nElements; i++) {
            int col = array->row[i].col;
            int val = array->row[i].val;

            // Skipping zero values
            if (val != EMPTY) {
                // Print spaces from the last seen
                printf("%*s%d", col - start, "", val);
                start = col + 1;
            }
        }
    }
    // print remaining spaces till the end of the row
    printf("%*s]", end - start, "");
    //if (array != NULL)
    //printf("%d\n", array->nElements);
    //else
    printf("\n");
}

void printCoordinate(row_t *array, int row) {
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

// Because we read the matrix in aribitray order.
// we need to first sort the element in each row.
void firstsort(CSRMatrix_t *A) {
    for (int i = 0; i < A->rows; i++) {
        row_t *arrInfo = A->rptr[i];
        if (arrInfo != NULL) {
            qsort(arrInfo->row, arrInfo->nElements, 
                                    sizeof(cell_t), colcmp);
        }
    }
}
void printMatrix(CSRMatrix_t *A) {
    for (int i = 0; i < A->rows; i++) {
        row_t *arrInfo = A->rptr[i];
        if (A->cols > PRINTABLE || A->rows > PRINTABLE || A->notMatrix) {
            printCoordinate(arrInfo, i);
        } else {
            printProperRow(arrInfo, A->cols);
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

void null_check(CSRMatrix_t *A, int index) {
    if (A->rptr[index] == NULL) {
        A->rptr[index] = create_arrayInfo(EMPTY, INIT_SIZE);
    }
}
// Get the cell at colval otherwise we append at the end of the row
search_t getOrApend(CSRMatrix_t *A, int rowval, int colval) {
    null_check(A, rowval);
    row_t *arrInfo = A->rptr[rowval];
    search_t res;

    int pos = binarySearch(arrInfo, colval, NULL);
    if (pos != NOTFOUND) {
        // We found the cell; 
        res.foundAt = res.appendAt = pos;
        return res;
    }

    append(arrInfo, arrInfo->nElements, rowval, colval);
    res.foundAt = NOTFOUND, res.appendAt = arrInfo->nElements - 1;
    return res;
}

void swap(int*a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
// Swap the cell at (r1, c1) with cell at (r2, c2)
void swap_cell(CSRMatrix_t *A, changeInfo_t *info) {
    // Write this again.
}
// Set the cell at (r1,c1) to value val
void set_cell(CSRMatrix_t *A, changeInfo_t *info) {
    if (info->val == 0 && A->rptr[info->r1] == NULL) return;
    row_t *arrInfo = A->rptr[info->r1];

    int inpos = NOTFOUND;
    // Index is the position of insertion and idx is the actual index of 
    // the cell
    search_t res = getOrApend(A, info->r1, info->c1);

    // Get prev before overwriting to update nnz and notMatrix
    int prev = arrInfo->row[res.appendAt].val;
    arrInfo->row[res.appendAt].val = info->val;
    notMatrix(prev, info->val, &A->notMatrix, &arrInfo->nonMatrix);  
    nonZeroCount(prev, info->val, &A->nnz, &arrInfo->nnz);
    move(arrInfo, res.appendAt, inpos);
}

void add_or_multi(CSRMatrix_t *A, changeInfo_t *info) {
    for (int i = 0; i < A->rows; i++) {
        if (A->rptr[i] == NULL) continue;
        row_t *arrInfo = A->rptr[i];

        for (int j = 0; j < arrInfo->nElements; j++) {
            int prev = arrInfo->row[j].val;
            int*curr = &arrInfo->row[j].val;
            if (info->add && *curr != EMPTY) {
                *curr += info->val;
            } else {
                *curr *= info->val;
            }
            notMatrix(prev, *curr, &A->notMatrix, &arrInfo->nonMatrix);
            nonZeroCount(prev, *curr, &A->nnz, &arrInfo->nnz);
            remove_zero(arrInfo, j);
        }
    }
}

// Swap the rows is simply swapping row pointers
void swap_row(CSRMatrix_t *A, changeInfo_t *info) {
    row_t *temp = A->rptr[info->r1];
    A->rptr[info->r1] = A->rptr[info->r2];
    A->rptr[info->r2] = temp;
}

void copy_row(CSRMatrix_t *A, changeInfo_t *info) {
    if (info->r1 == info->r2) return; // copying to itself, do nothing
    row_t *src = A->rptr[info->r1];
    row_t *target = A->rptr[info->r2];

    // Remove the target row first
    A->nnz -= (target != NULL) ? target->nnz : 0;
    A->notMatrix -= (target != NULL) ? target->nonMatrix : 0;
    free_arrayInfo(target);

    if (src == NULL) {
        A->rptr[info->r2] = NULL; 
        return;
    }
    row_t *new = create_arrayInfo(src->nElements, src->row_size);
    memcpy(new->row, src->row, sizeof(cell_t) * src->nElements);
    // copy the content of the source row to the new row
    A->rptr[info->r2] = new;
    A->nnz += A->rptr[info->r2]->nnz = src->nnz;
    A->notMatrix +=  A->rptr[info->r2]->nonMatrix = src->nonMatrix;
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
    swap(&info->c1, &info->c2);
    for (int i = 0; i < A->rows; i++) {
        row_t *row = A->rptr[i];
        int i1 = NOTFOUND;
        info->r1 = i;

        if (row) {
            i1 = binarySearch(row, info->c2, NULL);
            info->val = (i1 != NOTFOUND)? row->row[i1].val : EMPTY;
            set_cell(A, info);
        }
    }
}
void stage0(CSRMatrix_t *A, CSRMatrix_t *B) {
    firstsort(A);
    firstsort(B);
    matrixStatus(A, INIT); // print initial matrix

    printf("-------------------------------------\n");

    matrixStatus(B, TARGET); // print target matrix
}

// Check if the command is valid for stage 1
int stage1Change(char cmd) {
    return (cmd == 's' || cmd == 'S' || cmd == 'm' || cmd == 'a');
}

// Creating an array of change format and 
// function pointer for each command
change_t *assignChange() {
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

// A function that perform both stage 1 and 2.
//~~Read the command. IF we are still in STAGE1 and 
//~~command is of STAGE2, break the current reading.
//Scanf the changeform base on the command.
//Call the function pointer to perform the change.
// Print out the matrix after manipulation.
void changeReader(CSRMatrix_t *A, CSRMatrix_t *B, int stage, 
                                int *count, change_t *changes) {
    char command;
    changeInfo_t info;

    while (scanf("%c", &command) == 1) {

        if (stage == 1 && !stage1Change(command)) {
            ungetc(command, stdin);
            break;
        }

        char *changeForm = changes[(int)command].format;
        void (*func)(CSRMatrix_t*, changeInfo_t*) 
                            = changes[(int)command].func;
        //Decide Whether it is addition or multiplication
        // for add_or_multi function    
        info.add = (command == 'a') ? TRUE : FALSE;
        
        scanPrintInstruction(changeForm, command, &info);

        func(A, &info);

        matrixStatus(A, CURR);
        matrixStatus(B, TARGET);
        (*count)++;
        if (compareMatrix(A, B)) {
            theEnd(count);
            *count = NOTFOUND;
            break;
        }
    }
}

void theEnd(int *count) {
    printf("-------------------------------------\n");
    printf("TA-DAA!!! SOLVED IN %d STEP(S)!\n", *count);
}

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
        if (rowB == NULL) {
            for (int j = 0; j < rowA->nElements; j++) {
                if (rowA->row[j].val) {
                    return FALSE; // rowA has non-zero value
                }
            }
        }
        for (int j = 0; j < rowB->nElements; j++) {
            int index = binarySearch(rowA, rowB->row[j].col, NULL);
            if (index == NOTFOUND) {
                //printf("I Failed at row %d col %d with index = %d\n", i, rowB->row[j].col, index);
                return FALSE;  // column in rowB not found in rowA
            }
            if (rowA->row[index].val != rowB->row[j].val) {
                //printf("I Failed at row %d col %d with index = %d\n", i, rowB->row[j].col, index);
                return FALSE;  // values do not match
            }
        }
    }
    return TRUE;
}

