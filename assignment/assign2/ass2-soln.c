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

  Signed by: Artem Polyvyanyy
  Dated:     14 September 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define TADAAM "TA-DAA!!! SOLVED IN %d STEP(S)!\n"          // ta-daa! string

#define MTXDIM "%dx%d\n"        // matrix dimensions input format
#define DELFMT "%c\n"           // matrix instructions delimiter format
#define DELCHR '#'              // matrix instructions delimiter character
#define DIMFMT "%dx%d, nnz=%d\n"// matrix dimensions output format
#define OSBFMT "["              // opening square bracket format
#define DIGFMT "%d"             // decimal number format
#define SPCFMT " "              // space character format
#define CSBFMT "]\n"            // closing square bracket format
#define CVLFMT "(%d,%d)=%d\n"   // cell value printing format
#define RACLMT 36               // max rows/cols for printing as matrix
#define INICAP 4                // initial capacity for non-zero values
#define INCCAP 2                // matrix capacity increment factor
#define TRUEBV 1                // true Boolean value
#define FALSBV 0                // false Boolean value
#define MMTFMT "%c:"            // matrix manipulation type input format
#define SETMMT 's'              // set value matrix manipulation type
#define SWPMMT 'S'              // swap values matrix manipulation type
#define MULMMT 'm'              // multiply all entries matrix manipulation type
#define ADDMMT 'a'              // add to all nnzs matrix manipulation type
#define RCPMMT 'r'              // row copy matrix manipulation type
#define CCPMMT 'c'              // column copy matrix manipulation type
#define RSWMMT 'R'              // row swap matrix manipulation type
#define CSWMMT 'C'              // column swap matrix manipulation type

#define CMXFMT "Current matrix: "       // current matrix string
#define TMXFMT "Target matrix: "        // target matrix string

#define ONENUM "%d\n"                           // one number input format
#define TWONUM "%d,%d\n"                        // two numbers input format
#define THRNUM "%d,%d,%d\n"                     // three numbers input format
#define FOUNUM "%d,%d,%d,%d\n"                  // four numbers input format
#define ONEINS "INSTRUCTION %c:%d\n"            // one parameter instruction
#define TWOINS "INSTRUCTION %c:%d,%d\n"         // two parameters instruction
#define THRINS "INSTRUCTION %c:%d,%d,%d\n"      // three parameters instruction
#define FOUINS "INSTRUCTION %c:%d,%d,%d,%d\n"   // four parameters instruction

/* TYPE DEFINITIONS ----------------------------------------------------------*/
// Compressed Sparse Row (CSR) matrix representation
typedef struct {
    int  rows;       // number of rows in this matrix
    int  cols;       // number of columns in this matrix
    int  nnz;        // number of stored non-zeros values in this matrix
    int  cap;        // matrix capacity to hold non-zero values
    int* vals;       // non-zero values in this matrix
    int* cidx;       // column indices of non-zero values, in row-major order
    int* rptr;       // row pointers
} CSRMatrix;

/* FUNCTION PROTOTYPES -------------------------------------------------------*/

/* INTERFACE FUNCTIONS FOR WORKING WITH CSR MATRICES -------------------------*/
CSRMatrix*  csr_matrix_create(int, int);        // create empty CSR matrix
void        csr_matrix_free(CSRMatrix*);        // free input CSR matrix
void        csr_matrix_read(CSRMatrix*);        // read matrix from stdin
void        csr_matrix_print(const CSRMatrix*); // print matrix to stdout
            // get value in given row and column in matrix
int         csr_matrix_get(const CSRMatrix*, int, int);
            // set value in given row and column in matrix
void        csr_matrix_set(CSRMatrix*, int, int, int);
            // swap matrix values between two cells
void        csr_matrix_swap(CSRMatrix*, int, int, int, int);
            // multiply all values in the matrix by a number
void        csr_matrix_multiply(CSRMatrix*, int);
            // add a number to all non-zero values in the matrix
void        csr_matrix_add_to_nonzeros(CSRMatrix*, int);
            // swap two rows in matrix
void        csr_matrix_swap_rows(CSRMatrix*, int, int);
            // swap two columns in matrix
void        csr_matrix_swap_cols(CSRMatrix*, int, int);
            // check if two matrices are equal
int         csr_matrix_equal(const CSRMatrix*, const CSRMatrix *);
            // copy a row in the matrix
void        csr_matrix_copy_row(CSRMatrix*, int, int);
            // copy a column in the matrix
void        csr_matrix_copy_col(CSRMatrix*, int, int);

/* AUXILIARY FUNCTIONS FOR WORKING WITH CSR MATRICES -------------------------*/
            // find position in matrix vals where value in given
            // row and column is stored
static int  csr_matrix_find_position(const CSRMatrix*, int, int, int*);
            // ensure matrix can hold at least the given number of nnz values
static void csr_matrix_alloc(CSRMatrix*, int);
            // check if matrix values are in the 0-9 range
static int  csr_matrix_entries_in_range(const CSRMatrix*);

/* PROCESSING INSTRUCTIONS IN STAGES 1 AND 2 OF ASSIGNMENT 2 -----------------*/
static void process_instructions(CSRMatrix*, CSRMatrix*, int);

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(void) {
    int stage=0, rows, cols;
    printf(SDELIM, stage++);                        // print Stage 0 header
    assert(scanf(MTXDIM, &rows, &cols)==2);         // read matrix dimensions
    CSRMatrix* A = csr_matrix_create(rows, cols);   // create initial matrix
    CSRMatrix* B = csr_matrix_create(rows, cols);   // create target matrix
    csr_matrix_read(A);                             // read initial matrix
    csr_matrix_read(B);                             // read target matrix
    printf("Initial matrix: ");                     // print initial matrix
    csr_matrix_print(A);
    printf(MDELIM);                                 // print delimiter line
    printf("Target matrix: ");                      // print target matrix
    csr_matrix_print(B);
    process_instructions(A,B,stage);                // process instructions
    printf(THEEND);                                 // print "THE END" message
    csr_matrix_free(A);                             // free initial matrix
    csr_matrix_free(B);                             // free target matrix
    return EXIT_SUCCESS;                            // algorithms are fun!!!
}

/* FUNCTION DEFINITIONS ------------------------------------------------------*/

// Create an empty CSR  matrix of nrows rows and ncols columns
CSRMatrix *csr_matrix_create(int nrows, int ncols) {
    assert(nrows >= 0 && ncols >= 0);   // check matrix dimensions
    // allocate memory for this matrix
    CSRMatrix *A = (CSRMatrix*)malloc(sizeof(CSRMatrix));
    assert(A!=NULL);            // check memory was allocated
    A->rows = nrows;            // set number of rows in the matrix
    A->cols = ncols;            // set number of columns in the matrix
    A->nnz  = 0;                // initialize with no non-zero values
    A->cap  = 0;                // initialize capacity to no non-zero values
    A->vals = NULL;             // no values to store yet ...
    A->cidx = NULL;             // so there is no need to store column indices
    // allocate array to store row pointers
    A->rptr = (int*)malloc((size_t)(A->rows+1) * sizeof(int));
    assert(A->rptr!=NULL);
    for (int i = 0; i <= A->rows; i++) {    // no values, so initialize ...
        A->rptr[i] = 0;                     // ... all row pointers to zeros
    }
    return A;
}

// Free input CSR matrix A
void csr_matrix_free(CSRMatrix *A) {
    assert(A!=NULL);
    free(A->vals);      // free matrix values
    free(A->cidx);      // free column indices
    free(A->rptr);      // free row pointers
    free(A);            // free matrix
}

// Read a CSR matrix from standard input
void csr_matrix_read(CSRMatrix *A) {
    assert(A!=NULL);                // check if matrix exists
    int r, c, v;
    char ch;
    while (scanf(THRNUM, &r, &c, &v)==3) {  // while more instructions
        if (r < 0 || r >= A->rows || c < 0 || c >= A->cols) {
            continue;               // skip wrong instruction
        }
        csr_matrix_set(A, r, c, v); // set value of the (r,c)-cell in A to v
    }
    assert(scanf(DELFMT, &ch)==1 && ch==DELCHR);
}

// Print input CSR matrix A to standard output
void csr_matrix_print(const CSRMatrix *A) {
    assert(A!=NULL);                            // check if matrix exists
    printf(DIMFMT, A->rows, A->cols, A->nnz);   // print matrix dims and #nnz
    int val;
    if (A->rows < RACLMT && A->cols < RACLMT
            && csr_matrix_entries_in_range(A)) {// print as matrix
        for (int i = 0; i < A->rows; i++) {     // for each matrix row
            printf(OSBFMT);                     // print opening bracket
            for (int j=0; j<A->cols; j++) {     // for each matrix column
                val = csr_matrix_get(A,i,j);    // get value
                if (val) {
                    printf(DIGFMT,val);         // print nnz entry
                }
                else {
                    printf(SPCFMT);             // print space for zero entry
                }
            }
            printf(CSBFMT);
        }
    }
    else {                                      // print as list of nnz entries
        for (int i=0; i < A->rows; i++) {       // for each matrix row
            for (int p=A->rptr[i]; p < A->rptr[i+1]; p++) {
                // print nnz entry
                printf(CVLFMT, i, A->cidx[p], A->vals[p]);
            }
        }
    }
}

// Get value in row i and column j in CSR matrix A
int csr_matrix_get(const CSRMatrix *A, int i, int j) {
    assert(i >= 0 && i < A->rows && j >= 0 && j < A->cols);
    int p;  // position in A->vals where value in row i and column j is stored
    if (csr_matrix_find_position(A, i, j, &p)) {
        return A->vals[p];
    }
    return 0;
}

// Set value v in row i and column j in matrix A
void csr_matrix_set(CSRMatrix *A, int i, int j, int v) {
    assert(i >= 0 && i < A->rows && j >= 0 && j < A->cols);
    int p;
    if (csr_matrix_find_position(A, i, j, &p)) {
        // current value in A in row i and column j is a non-zero value
        if (!v) {               // set zero value
            // delete entry at position p
            // shift left column indices and values starting from position p+1
            memmove(&A->cidx[p],&A->cidx[p+1],(size_t)(A->nnz-p-1)*sizeof(int));
            memmove(&A->vals[p],&A->vals[p+1],(size_t)(A->nnz-p-1)*sizeof(int));
            A->nnz--;           // decrement number of nnz values
            for (int r=i+1; r <= A->rows; r++) {
                A->rptr[r]--;   // decrement rptr for rows after i
            }
        } else {
            A->vals[p] = v;     // set nnz value
        }
        return;                 // done updating non-zero entry
    }
    if (!v) {
        return;                 // no need to set zero value
    }
    // insert v at position p
    // ensure sufficient capacity and then shift right
    csr_matrix_alloc(A, A->nnz+1);
    memmove(&A->cidx[p+1],&A->cidx[p],(size_t)(A->nnz-p)*sizeof(int));
    memmove(&A->vals[p+1],&A->vals[p],(size_t)(A->nnz-p)*sizeof(int));
    A->cidx[p] = j;             // set column index
    A->vals[p] = v;             // set value
    A->nnz++;                   // increment number of nnz values
    for (int r=i+1; r <= A->rows; r++) {
        A->rptr[r]++;           // increment rptr for rows after i
    }
}

// Swap values between cells (r1,c1) and (r2,c2) in matrix A
void csr_matrix_swap(CSRMatrix *A, int r1, int c1, int r2, int c2) {
    assert(A != NULL);          // check if matrix exists
    // check if cells exist
    assert(r1 >= 0 && r1 < A->rows && c1 >= 0 && c1 < A->cols);
    assert(r2 >= 0 && r2 < A->rows && c2 >= 0 && c2 < A->cols);
    if (r1 == r2 && c1 == c2) {
        return;                 // same cell -> nothing to do
    }
    int v1 = csr_matrix_get(A, r1, c1);     // get value in cell (r1,c1)
    int v2 = csr_matrix_get(A, r2, c2);     // get value in cell (r2,c2)
    if (v1 == v2) {
        return;                 // same value -> nothing to do
    }
    csr_matrix_set(A, r1, c1, v2);          // set value in cell (r1,c1)
    csr_matrix_set(A, r2, c2, v1);          // get value in cell (r2,c2)
}

// Multiply all values in the matrix by a constant 'factor'
void csr_matrix_multiply(CSRMatrix *A, int factor) {
    assert(A != NULL);          // check if matrix exists
    if (factor == 1) {
        return;                 // no change -> nothing to do
    }
    if (factor == 0) {
        // set all values to zero and reset nnz and row pointers
        A->nnz = 0;
        for (int i=0; i <= A->rows; i++) {
            A->rptr[i] = 0;
        }
        return;
    }
    // multiply each non-zero entry by the given 'factor'
    for (int p=0; p < A->nnz; p++) {
        A->vals[p] *= factor;
    }
}

// Add a constant 'delta' to all non-zero values in the matrix
void csr_matrix_add_to_nonzeros(CSRMatrix *A, int delta) {
    assert(A != NULL);          // check if matrix exists
    if (delta == 0) {
        return;                 // no change -> nothing to do
    }
    // add 'delta' to each non-zero entry
    for (int p=0; p < A->nnz; p++) {
        A->vals[p] += delta;
    }
}

// Swap two rows r1 and r2 in CSR matrix A
void csr_matrix_swap_rows(CSRMatrix *A, int r1, int r2) {
    assert(A != NULL);          // check if matrix exists
    // check if rows exist
    assert(r1 >= 0 && r1 < A->rows && r2 >= 0 && r2 < A->rows);
    if (r1 == r2) {
        return;                 // same row -> nothing to do
    }
    // remember start and end of both rows
    int s1=A->rptr[r1], e1=A->rptr[r1+1], l1=e1-s1;
    int s2=A->rptr[r2], e2=A->rptr[r2+1], l2=e2-s2;
    // allocate memory to store column indices and values in both rows
    int *c1=l1 ? (int*)malloc((size_t)l1*sizeof(int)) : NULL;
    int *v1=l1 ? (int*)malloc((size_t)l1*sizeof(int)) : NULL;
    int *c2=l2 ? (int*)malloc((size_t)l2*sizeof(int)) : NULL;
    int *v2=l2 ? (int*)malloc((size_t)l2*sizeof(int)) : NULL;
    // copy column indices and values into temporary storage
    for (int k=0; k < l1; k++) { c1[k]=A->cidx[s1+k]; v1[k]=A->vals[s1+k]; }
    for (int k=0; k < l2; k++) { c2[k]=A->cidx[s2+k]; v2[k]=A->vals[s2+k]; }
    // swap rows r1 and r2
    for (int k=l2-1; k >= 0; k--) csr_matrix_set(A, r2, c2[k], 0);
    for (int k=l1-1; k >= 0; k--) csr_matrix_set(A, r1, c1[k], 0);
    for (int k=0; k < l2; k++) csr_matrix_set(A, r1, c2[k], v2[k]);
    for (int k=0; k < l1; k++) csr_matrix_set(A, r2, c1[k], v1[k]);
    // free temporary storage of column indices and values
    free(c1); free(v1); free(c2); free(v2);
}

// Swap two columns c1 and c2 in CSR matrix A
void csr_matrix_swap_cols(CSRMatrix *A, int c1, int c2) {
    assert(A != NULL);          // check if matrix exists
    // check if columns exist
    assert(c1 >= 0 && c1 < A->cols && c2 >= 0 && c2 < A->cols);
    if (c1 == c2) {
        return;                 // same column -> nothing to do
    }
    for (int i = 0; i < A->rows; ++i) {
        int v1 = csr_matrix_get(A, i, c1);  // get value in column c1
        int v2 = csr_matrix_get(A, i, c2);  // get value in column c2
        if (v1 == v2) {
            continue;           // same values -> nothing to do
        }
        csr_matrix_set(A, i, c1, v2);       // set value in column c1
        csr_matrix_set(A, i, c2, v1);       // set value in column c2
    }
}

// Check if two CSR matrices A and B are identical
int csr_matrix_equal(const CSRMatrix *A, const CSRMatrix *B) {
    assert(A!=NULL && B!=NULL); // check if matrices exist
    // compare dimensions and number of non-zeros
    if (A->rows != B->rows || A->cols != B->cols || A->nnz != B->nnz) {
        return FALSBV;          // A and B are different
    }
    // compare row pointers (numbers of non-zero values in rows)
    for (int i=0; i <= A->rows; i++) {
        if (A->rptr[i] != B->rptr[i]) {
            return FALSBV;      // A and B are different
        }
    }
    // compare column indices and values (final comparison)
    for (int k=0; k < A->nnz; k++) {
        if (A->cidx[k] != B->cidx[k]) {
            return FALSBV;      // A and B are different
        }
        if (A->vals[k] != B->vals[k]) {
            return FALSBV;      // A and B are different
        }
    }
    return TRUEBV;
}

// Copy values in row 'src' to row 'dst' in CSR matrix A
void csr_matrix_copy_row(CSRMatrix *A, int src, int dst) {
    assert(A != NULL);          // check if matrix exists
    // check if columns exist
    assert(src >= 0 && src < A->rows && dst >= 0 && dst < A->rows);
    if (src == dst) {
        return;                 // same rows -> nothing to do
    }
    for (int i=0; i < A->cols; i++) {
        int v = csr_matrix_get(A, src, i);      // get value
        csr_matrix_set(A, dst, i, v);           // set value
    }
}

// Copy values in column 'src' to column 'dst' in CSR matrix A
void csr_matrix_copy_col(CSRMatrix *A, int src, int dst) {
    assert(A != NULL);          // check if matrix exists
    // check if columns exist
    assert(src >= 0 && src < A->cols && dst >= 0 && dst < A->cols);
    if (src == dst) {
        return;                 // same columns -> nothing to do
    }
    for (int i=0; i < A->rows; i++) {
        int v = csr_matrix_get(A, i, src);      // get value
        csr_matrix_set(A, i, dst, v);           // set value
    }
}

// Find position p in A->vals where the value in row i and column j is stored
static int csr_matrix_find_position(
                        const CSRMatrix *A, int i, int j, int *p) {
    // position of first column index with non-zero value in row i
    int lo = A->rptr[i];
    // position of first column index with non-zero value in row i+1
    int hi = A->rptr[i+1];
    // perform  binary search on column indices with non-zero values in row i
    // to find position in A->vals with a non-zero value
    while (lo < hi) {
        int mid = lo+(hi-lo)/2;         // pick middle position
        int c = A->cidx[mid];           // get column index at middle position
        if (c < j) {                    // if smaller than required
            lo = mid+1;                 // update left position boundary
        }
        else {
            hi = mid;                   // update right position boundary
        }
    }
    *p = lo;                            // store position
    // return whether a position with a non-zero value was found
    return (lo < A->rptr[i+1] && A->cidx[lo] == j);
}

// Ensure CSR matrix A can hold at least 'need' non-zero values
static void csr_matrix_alloc(CSRMatrix *A, int need) {
    if (need <= A->cap) return; // A can already hold 'need' non-zero values
    int newcap = A->cap ? A->cap : INICAP;
    while (newcap < need) {     // need more capacity?
        newcap *= INCCAP;       // increase the capacity
    }
    // reallocate arrays with column indices and non-zero values
    A->vals = (int*) realloc(A->vals, (size_t)newcap * sizeof(int));
    assert(A->vals!=NULL);
    A->cidx = (int*) realloc(A->cidx, (size_t)newcap * sizeof(int));
    assert(A->cidx!=NULL);
    A->cap = newcap;
}

// Check if values in matrix A are in the 0-9 range
static int csr_matrix_entries_in_range(const CSRMatrix *A) {
    assert(A!=NULL);            // check if matrix exists
    for (int i=0; i<A->nnz; i++) {
        if (A->vals[i]<0 || A->vals[i]>9) {
            return FALSBV;      // not in the 0-9 range
        }
    }
    return TRUEBV;              // in the 0-9 range
}

// Process matrix manipulation instructions for initial A and target B matrices
static void process_instructions(CSRMatrix *A, CSRMatrix *B, int stage) {
    char ch;
    int r1, r2, c1, c2, n1, n2, v, count=0;
    printf(SDELIM, stage++);                // print stage delimiter
    while (scanf(MMTFMT,&ch)==1) {          // while new manipulation provided
        if (ch==SETMMT && scanf(THRNUM,&r1,&c1,&v)==3) {
            csr_matrix_set(A,r1,c1,v);      // set value
            printf(THRINS,ch,r1,c1,v);
        }
        else if (ch==SWPMMT && scanf(FOUNUM,&r1,&c1,&r2,&c2)==4) {
            csr_matrix_swap(A,r1,c1,r2,c2); // swap values
            printf(FOUINS,ch,r1,c1,r2,c2);
        }
        else if (ch==MULMMT && scanf(ONENUM,&v)==1) {
            csr_matrix_multiply(A,v);       // multiply all entries
            printf(ONEINS,ch,v);
        }
        else if (ch==ADDMMT && scanf(ONENUM,&v)==1) {
            csr_matrix_add_to_nonzeros(A,v);// add to non-zero entries
            printf(ONEINS,ch,v);
        }
        else{   // process Stage 2 matrix manipulations
            if (stage<3) printf(SDELIM, stage++);
            if (scanf(TWONUM,&n1,&n2)==2) {
                if (ch==RCPMMT) {           // copy row
                    csr_matrix_copy_row(A, n1, n2);
                }
                else if (ch==CCPMMT) {      // copy column
                    csr_matrix_copy_col(A, n1, n2);
                }
                else if (ch==RSWMMT) {      // swap rows
                    csr_matrix_swap_rows(A, n1, n2);
                }
                else if (ch==CSWMMT) {      // swap columns
                    csr_matrix_swap_cols(A, n1, n2);
                }
                else return;
            }
            else return;
            printf(TWOINS,ch,n1,n2);
        }
        count++;                        // count performed manipulations
        // Print current and target matrices
        printf(CMXFMT);
        csr_matrix_print(A);
        printf(TMXFMT);
        csr_matrix_print(B);
        if (csr_matrix_equal(A,B)) {    // if initial and target matrices ...
            printf(MDELIM);             // are identical, print the delimiter
            printf(TADAAM,count);       // and the ta-daa! message
            return;
        }
    }
}
