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
  Student General Misconduct may arise regardless of whether or not I 
  personally make use of such solutions or sought benefit from such actions.

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
#define INITIAL_MAP 4 // Initial number of manipulations can hold
#define INITIAL_ENTRIES 1000 // Initial number of entries can hold

/* TYPE DEFINITIONS ----------------------------------------------------------*/
// Compressed Sparse Row (CSR) matrix representation
typedef struct {
    int  rows;       // number of rows in this matrix
    int  cols;       // number of columns in this matrix
    int  nnz;        // number of stored non-zeros values in this matrix
    int  cap;        // matrix capacity to hold non-zero values
    int* vals;       // non-zero values in this matrix
    int* ridx;       // Row indices of non-zero values
    int* cidx;       // column indices of non-zero values, in row-major order
    int* rptr;       // row pointers
} CSRMatrix_t;

typedef struct {
    char type[MANIPULATION_CHAR];
    int para1, para2, para3, para4; // 4 parameters (set 0 for unused para)
    int num_map; // Number of manipulations
} Manip_t;

/* FUNCTION PROTOTYPES -------------------------------------------------------*/
Manip_t *do_stage_0(int rows,int cols,
    CSRMatrix_t* A, CSRMatrix_t* B,int *stage); 
void do_stage_1(Manip_t* manip, CSRMatrix_t* A, CSRMatrix_t* B,int *stage);
/* INTERFACE FUNCTIONS FOR WORKING WITH CSR MATRICES -------------------------*/
CSRMatrix_t*  csr_matrix_create(int, int);        // create empty CSR matrix
void          csr_matrix_free(CSRMatrix_t*);      // free input CSR matrix
void read_input(CSRMatrix_t* A, int rows);
Manip_t *read_manip(void);
void print_matrix(CSRMatrix_t* A);
// Check if initial matrix match target
int match(CSRMatrix_t* A, CSRMatrix_t* B); 
void shift_arr(CSRMatrix_t* A, int pos, int direction);
/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(void) {
    int stage=0, rows, cols;
    assert(scanf(MTXDIM, &rows, &cols)==2);       // assert matrix dimensions
    CSRMatrix_t* A = csr_matrix_create(rows,cols);// create initial matrix of 0s
    CSRMatrix_t* B = csr_matrix_create(rows,cols);// create target matrix of 0s
    Manip_t *manip = do_stage_0(rows, cols, A, B, &stage);               
    // ...
    printf(SDELIM, stage++);                      // print Stage 1 header
    do_stage_1(manip, A, B, &stage);
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
    A->ridx = NULL;             // so there is no need to store row indices
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
    free(A->ridx);
    free(A->vals);      // free matrix values
    free(A->cidx);      // free column indices
    free(A->rptr);      // free row pointers
    free(A);            // free matrix
}

// Stage 0 required: 
Manip_t* do_stage_0(int rows, int cols, CSRMatrix_t* A,
    CSRMatrix_t* B,int *stage) {
    assert(A!=NULL & B!=NULL);
    // upper bound limit (realloc more efficient here?)
    A->vals = (int*)malloc(INITIAL_ENTRIES*sizeof(int));
    A->ridx = (int*)malloc(INITIAL_ENTRIES*sizeof(int));
    A->cidx = (int*)malloc(INITIAL_ENTRIES*sizeof(int));
    assert(A->cidx!=NULL && A->ridx!=NULL && A->vals!=NULL);

    B->vals = (int*)malloc(INITIAL_ENTRIES*sizeof(int));
    B->ridx = (int*)malloc(INITIAL_ENTRIES*sizeof(int));
    B->cidx = (int*)malloc(INITIAL_ENTRIES*sizeof(int));
    assert(A->cidx!=NULL && A->ridx!=NULL && A->vals!=NULL);

    read_input(A, rows);
    read_input(B, rows);
    Manip_t *manip = read_manip();

    printf("\n");
    printf(SDELIM, (*stage)++);

    printf("Initial matrix: %dx%d, nnz=%d\n", rows, cols, A->nnz);
    print_matrix(A);
    printf(LINESEP);
    printf("Target matrix: %dx%d, nnz=%d\n", rows, cols, B->nnz);
    print_matrix(B);

    return manip;
}

// Print out the matrix using CSR matrix
void print_matrix(CSRMatrix_t* A) {
    assert(A!=NULL);
    if (A->rows > LIST_OUTPUT || A->cols > LIST_OUTPUT) {
        for (int row = 0; row < A->nnz; row++) {
            printf("(%d,%d)=%d\n", A->ridx[row],A->cidx[row],A->vals[row]);
        }
    }
    else {
        for (int row = 0; row < A->rows; row++) {
            printf("[");
            // If end-start>0 then following row will have values
            int start = A->rptr[row];
            int end = A->rptr[row+1];
            for (int col = 0; col < A->cols; col++) {
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
}   

// Reading input "r,s,v" format (Since input can be not in order for row)
// Using temporary pointer to handle random input
void read_input(CSRMatrix_t* A, int rows) {
    assert(A!=NULL);
    int row, col, value, ch;
    int cur_size = INITIAL_ENTRIES;
    A->cap=1;
    while (scanf("%d,%d,%d", &row, &col, &value) == 3) {
        if (A->cap == cur_size) {
            cur_size *= 2;
            A->ridx = realloc(A->ridx, cur_size * sizeof(int)); 
            assert(A->ridx!=NULL); 
            A->cidx = realloc(A->cidx, cur_size * sizeof(int)); 
            assert(A->cidx!=NULL); 
            A->vals = realloc(A->vals, cur_size * sizeof(int)); 
            assert(A->vals!=NULL);
            
        }
        // Compute CSR Matrix 
        A->ridx[A->nnz] = row;
        A->cidx[A->nnz] = col;
        A->rptr[row]++;
        A->vals[A->nnz] = value;
        A->nnz++;
        A->cap++;
        // read one char right after the triplet
        ch = getchar();

        if (ch == '\n' || ch == '\r' || ch == ' ') {
            // read ahead one more character to see whether it's "#"
            ch = getchar();
        }
        
        if (ch == '#') {
            break;
        }
        // otherwise push it back for scanf
        ungetc(ch, stdin);
    }
    A->cap = cur_size;
    // Add all the accumulation to get row pointer correct
    // It will store how many values in row 0 to i - 1 for position i
    int accumulate = 0;
    for (int i = 0; i <= rows; i++) {
        int cnt = A->rptr[i];          // Set entries row i has
        A->rptr[i] = accumulate;       // set start index for this row
        accumulate += cnt;             // accumulate
    }

    // Scatter into correct row slices (stable across input order)
    int *next = (int*)malloc(A->rows * sizeof(int));
    assert(next != NULL);
    for (int r = 0; r < A->rows; r++) { // r for row
        next[r] = A->rptr[r];
    }
    
    // Use temporary hold to deal with non-order input like test1
    int *tmp_cidx = (int*)malloc(A->nnz * sizeof(int));
    int *tmp_vals = (int*)malloc(A->nnz * sizeof(int));
    assert(tmp_cidx && tmp_vals);

    // For each nonzero (row, col, value), find the next free position 
    // for its row in the final matrix arrays and place it there
    for (int i = 0; i < A->nnz; i++) {
        int r = A->ridx[i];
        int pos = next[r]++; // next available position 
        tmp_cidx[pos] = A->cidx[i];
        tmp_vals[pos] = A->vals[i];
    }

    free(next);
    // Swap in row-grouped arrays (ordered)
    free(A->cidx); A->cidx = tmp_cidx; // points to new array
    free(A->vals); A->vals = tmp_vals; // points to new array
}

// Reading manipulation instructions
Manip_t *read_manip(void) {
    int cur_size = INITIAL_MAP; 
    Manip_t *manip = (Manip_t*)malloc(cur_size*sizeof((*manip)));
    assert(manip!=NULL);
    manip->num_map = 0;

    char ch;
    int i=0;
    while (scanf(" %c", &ch) == 1) {
        if (manip->num_map == cur_size) {
            cur_size *= 2;
            manip = realloc(manip, cur_size*sizeof(*manip));
            assert(manip!=NULL);
        }
        
        if (ch=='s') {
            scanf(":%d,%d,%d",&manip[i].para1,&manip[i].para2,&manip[i].para3);
        }
        else if (ch=='S') {
            scanf(":%d,%d,%d,%d", &manip[i].para1, 
                &manip[i].para2, &manip[i].para3, &manip[i].para4);
        }
        else if (ch=='m' || ch=='a') {
            scanf(":%d", &manip[i].para1);
        }
        else if (ch=='r' || ch=='c' || ch=='R' || ch=='C') {
            
            scanf(":%d,%d", &manip[i].para1, &manip[i].para2);
        }
        manip[i].type[0] = ch;
        manip[i].type[1] = '\0';
        manip->num_map++;
        i++;
        
    }
    return manip;

}
// Check if initial matrix match target matrix
int match(CSRMatrix_t *A, CSRMatrix_t *B) {
    // Check number of non-zero elements
    if (A->nnz != B->nnz) {
        return 0;  
    }

    // Check row pointers
    for (int i = 0; i <= A->rows; i++) {
        if (A->rptr[i] != B->rptr[i]) {
            return 0;   
        }
    }

    // Check column indices and values
    for (int i = 0; i < A->nnz; i++) {
        if (A->cidx[i] != B->cidx[i]) return 0;
        if (A->vals[i] != B->vals[i]) return 0;
    }

    return 1;  // all checks passed mean matrices are the same
}

void shift_arr(CSRMatrix_t* A, int pos, int direction) {
    // Check if size of array need to be realloc
    if (direction && A->nnz >= A->cap) {
        A->cap *= 2;
        A->cidx = realloc(A->cidx, A->cap * sizeof(int)); 
        assert(A->cidx!=NULL); 
        A->vals = realloc(A->vals, A->cap * sizeof(int)); 
        assert(A->vals!=NULL);
    }
    if (direction) {
        // Shift elements in array to the right
        for (int i = A->nnz; i > pos; i--) {
            A->cidx[i] = A->cidx[i - 1];
            A->vals[i] = A->vals[i - 1];
        }  
    }else {
        // Shift left
        for (int i = pos; i < A->nnz - 1; i++) {
            A->cidx[i] = A->cidx[i+1];
            A->vals[i] = A->vals[i+1];
        }  
    }
     
}
// Perform basic matrix manipulations
void do_stage_1(Manip_t* manip,CSRMatrix_t* A,CSRMatrix_t* B,int *stage) {
    // Apply all manipulations type until initial match target
    int row, col, val;
    // int row1, row2, col1, col2;
    for (int i=0;i < manip->num_map;i++) {
        if (strcmp(manip[i].type, "s") == 0) {
            row = manip[i].para1;
            col = manip[i].para2;
            val = manip[i].para3;
            printf("INSTRUCTION %s:%d,%d,%d\n", manip[i].type, row, col, val);
            // Slice of number of values in that row
            int start = A->rptr[row];
            int end = A->rptr[row+1];  
            int found = 0;
            for (int j = start; j < end; j++) {
                if (A->cidx[j] == col) {
                    if (val == 0) {
                        shift_arr(A, j, 0);
                        for (int i = j+1; i < A->rows; i++) {
                            A->rptr[i]--;
                        }
                        A->nnz--;
                    }
                    else {
                        A->vals[j] = val; // update existing value
                    }
                    found = 1;
                    break;
                }
            }

            // Insert new value if not found
            if (!found && val != 0) {
                // find where to insert to keep columns sorted
                int pos = start;
                while (pos < end && A->cidx[pos] < col) pos++;
                
                // shift to make room
                shift_arr(A, pos, 1);
                // insert new element
                A->cidx[pos] = col;
                A->vals[pos] = val;
                A->nnz++;
                // update row pointers
                for (int i = row+1; i < A->rows; i++) {
                    A->rptr[i]++;
                }
            }
            printf("Current Matrix: %dx%d, nnz=%d\n", A->rows,A->cols,A->nnz);
            print_matrix(A);
            printf("Current Matrix: %dx%d, nnz=%d\n", B->rows,B->cols,B->nnz);
            print_matrix(B);
        }
        else if (strcmp(manip[i].type, "S") == 0) {

        }
        else if (strcmp(manip[i].type, "m") == 0) {

        }
    }
    
}