/* Program that transforms a given initial two-dimensional matrix into a target
  matrix by applying a sequence of matrix manipulations.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/, 
  September 2025, with the intention that it be modified by students to add 
  functionality, as required by the assignment specification. All included 
  code is (c) Copyright University of Melbourne, 2025.

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
#define LIST_OUTPUT 35 // Output as list if rows or cols are larger than 35
#define MANIPULATION_CHAR 2 // manipulation of type 1 char + '/0'

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

typedef struct {
    char type[MANIPULATION_CHAR];
    int para1, para2, para3, para4;
} Manip_t;

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
void do_stage_0(int rows, int cols, CSRMatrix_t* A, CSRMatrix_t* B,int *stage); 
void do_stage_1(int rows, int cols, CSRMatrix_t* A, CSRMatrix_t* B,int *stage);
/* INTERFACE FUNCTIONS FOR WORKING WITH CSR MATRICES -------------------------*/
CSRMatrix_t*  csr_matrix_create(int, int);        // create empty CSR matrix
void          csr_matrix_free(CSRMatrix_t*);      // free input CSR matrix
void read_input(CSRMatrix_t* A, int rows);
void print_matrix(CSRMatrix_t* A, int rows, int cols);
/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(void) {
    int stage=0, rows, cols;
    assert(scanf(MTXDIM, &rows, &cols)==2);       // assert matrix dimensions
    CSRMatrix_t* A = csr_matrix_create(rows,cols);// create initial matrix of 0s
    CSRMatrix_t* B = csr_matrix_create(rows,cols);// create target matrix of 0s
    do_stage_0(rows, cols, A, B, &stage);               
    do_stage_1(rows, cols, A, B, &stage);
    // ...
    printf(SDELIM, stage++);                      // print Stage 1 header
    printf(SDELIM, stage++);                      // print Stage 2 header
    printf(THEEND);                               // print "THE END" message
    csr_matrix_free(A);                           // free initial matrix
    csr_matrix_free(B);                           // free target matrix
    
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

// Stage 0 required: 
void do_stage_0(int rows, int cols, CSRMatrix_t* A,CSRMatrix_t* B,int *stage) {
    assert(A!=NULL);
    // upper bound limit (realloc more efficient here?)
    int max_size = (rows)*(cols);
    A->vals = (int*)malloc(max_size*sizeof(int));
    assert(A->vals!=NULL);
    A->cidx = (int*)malloc(max_size*sizeof(int));
    assert(A->cidx!=NULL);

    B->vals = (int*)malloc(max_size*sizeof(int));
    assert(B->vals!=NULL);
    B->cidx = (int*)malloc(max_size*sizeof(int));
    assert(B->cidx!=NULL);

    read_input(A, rows);
    read_input(B, rows);
    printf("\n");
    printf(SDELIM, (*stage)++);

    printf("Initial matrix: %dx%d, nnz=%d\n", rows, cols, A->nnz);
    print_matrix(A, rows, cols);
    printf(LINESEP);
    printf("Target matrix: %dx%d, nnz=%d\n", rows, cols, B->nnz);
    print_matrix(B, rows, cols);

    return;
}

// Print out the matrix using CSR matrix way
void print_matrix(CSRMatrix_t* A, int rows, int cols) {
    for (int row = 0; row < rows; row++) {
        printf("[");
        // If end-start>0 then following row will have values
        int start = A->rptr[row];
        int end = A->rptr[row+1];
        for (int col = 0; col < cols; col++) {
            int found = 0;
            // Since cidx not sorted for each rows so again iterate to find
            // if col match that A->cidx[index] (prevent skipping values)
            for (int index = start; index < end; index++) {
                if (A->cidx[index] == col) {
                    printf("%d", A->vals[index]);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf(" ");   
            }
        }
        printf("]\n");
    }
}

// Reading input "r,s,v" format
void read_input(CSRMatrix_t* A, int rows) {
    assert(A!=NULL);
    int row, col, value, c;
    while (scanf("%d,%d,%d", &row, &col, &value) == 3) {
        // Compute CSR Matrix 
        A->cidx[A->nnz] = col;
        A->rptr[row]++;
        A->vals[A->nnz] = value;
        A->nnz++;
        // read one char right after the triplet
        c = getchar();

        if (c == '\n' || c == '\r' || c == ' ') {
            // read ahead one more character
            c = getchar();
        }
        // Break if next input is #
        if (c == '#') {
            break;
        }
        // otherwise push it back for scanf
        ungetc(c, stdin);
    }
    // Add all the accumulation to get row pointer correct
    
    int accumulate = 0;
    for (int i = 0; i <= rows; i++) {
        int cnt = A->rptr[i];   
        A->rptr[i] = accumulate;       // set start index for this row
        accumulate += cnt;             // accumulate
    }
    
}

//
void do_stage_1(int rows, int cols, CSRMatrix_t* A, CSRMatrix_t* B,int *stage) {
    
}