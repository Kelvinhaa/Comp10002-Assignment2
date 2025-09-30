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

  Signed by: Minh Hoang Ha
  Dated:     28 September 2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define THEEND "==THE END============================\n"    // end message
#define LINESEP "-------------------------------------\n"   // Line separation
#define MTXDIM "%dx%d\n"                // matrix dimensions input format

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
} CSRMatrix_t;

/* FUNCTION PROTOTYPES -------------------------------------------------------*/

/* INTERFACE FUNCTIONS FOR WORKING WITH CSR MATRICES -------------------------*/
CSRMatrix_t*  csr_matrix_create(int, int);        // create empty CSR matrix
void          csr_matrix_free(CSRMatrix_t*);      // free input CSR matrix
void          read_input(int rows, int cols, CSRMatrix_t* A); 
/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(void) {
    int stage=0, rows, cols;
    assert(scanf(MTXDIM, &rows, &cols)==2);       // assert matrix dimensions
    CSRMatrix_t* A = csr_matrix_create(rows,cols);// create initial matrix of 0s
    CSRMatrix_t* B = csr_matrix_create(rows,cols);// create target matrix of 0s
    read_input(rows, cols, A);
    read_input(rows, cols, B);
    printf(SDELIM, stage++);                      // print Stage 0 header
    printf(LINESEP);
    // ...
    printf(SDELIM, stage++);                      // print Stage 1 header
    printf(SDELIM, stage++);                      // print Stage 2 header
    printf(THEEND);                               // print "THE END" message
    csr_matrix_free(A);                           // free initial matrix
    csr_matrix_free(B);                           // free target matrix
    printf("algorithms are fun\n");
    return EXIT_SUCCESS;                          // algorithms are fun!!!
}

// Create an empty CSR  matrix of nrows rows and ncols columns
CSRMatrix_t *csr_matrix_create(int nrows, int ncols) {
    assert(nrows >= 0 && ncols >= 0);   // check matrix dimensions
    // allocate memory for this matrix
    CSRMatrix_t *A = (CSRMatrix_t*)malloc(sizeof(CSRMatrix_t));
    assert(A!=NULL);            // check if memory was allocated
    A->rows = nrows;            // set number of rows in the matrix
    A->cols = ncols;            // set number of columns in the matrix
    A->nnz  = 0;                // initialize with no non-zero values
    A->cap  = 0;                // initialize capacity to no non-zero values
    A->vals = NULL;             // no values to store...
    A->cidx = NULL;             // so there is no need to store column indices
    // allocate array to store row pointers
    A->rptr = (int*)malloc((size_t)(A->rows+1)*sizeof(int));
    assert(A->rptr!=NULL);
    for (int i = 0; i <= A->rows; i++) {    // no values, so initialize ...
        A->rptr[i] = 0;                     // ... all row pointers to zeros
    }
    return A;
}

// Free input CSR matrix A
void csr_matrix_free(CSRMatrix_t *A) {
    assert(A!=NULL);
    free(A->vals);      // free matrix values
    free(A->cidx);      // free column indices
    free(A->rptr);      // free row pointers
    free(A);            // free matrix
}

// Read matrices input
void read_input(int rows, int cols, CSRMatrix_t* A) {
    assert(A!=NULL);
    int row, col, value;
    int i=0;
    A->vals = (int*)malloc((size_t)(A->rows+1)*sizeof(int));
    assert(A->vals!=NULL);
    A->cidx = (int*)malloc((size_t)(cols)*sizeof(int));
    assert(A->cidx!=NULL);
    while(scanf("%d,%d,%d", &row, &col, &value)==3) {
        A->vals[i] = value;
        A->nnz++;
        A->cidx[i] = col;
        A->rptr[col]++;
    }
    return;
}