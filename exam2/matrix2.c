/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct
{
    double **data;
    int rows, cols;
} Matrix;


// Makes a new matrix and sets all elements to zero.
Matrix *make_matrix(int rows, int cols)
{
    int i;
    Matrix *matrix = malloc(sizeof(Matrix));

    matrix->data = malloc(rows * sizeof(double *));
    matrix->rows = rows;
    matrix->cols = cols;

    for (i = 0; i < rows; i++)
    {
        // use calloc to initialize to 0
        matrix->data[i] = calloc(cols, sizeof(double));
    }
    return matrix;
}

// Prints the elements of a matrix.
void print_matrix(Matrix *matrix)
{
    int i, j;

    for (i = 0; i < matrix->rows; i++)
    {
        for (j = 0; j < matrix->cols; j++)
        {
            printf("%lf ", matrix->data[i][j]);
        }
        printf("\n");
    }
}

// Adds a scalar to all elements of a matrix.
void increment_matrix(Matrix *matrix, int incr)
{
    int i, j;

    for (i = 0; i < matrix->rows; i++)
    {
        for (j = 0; j < matrix->cols; j++)
        {
            matrix->data[i][j] += incr;
        }
    }
}

// Sets the elements of a matrix to consecutive numbers.
void consecutive_matrix(Matrix *matrix)
{
    int i, j;

    for (i = 0; i < matrix->rows; i++)
    {
        for (j = 0; j < matrix->cols; j++)
        {
            matrix->data[i][j] = i * matrix->cols + j;
        }
    }
}

// Adds two matrices elementwise and stores the result in the given
// destination matrix (C).
void add_matrix(Matrix *A, Matrix *B, Matrix *C)
{
    int i, j;

    assert(A->rows == B->rows && B->rows == C->rows);
    assert(A->cols == B->cols && B->cols == C->cols);

    for (i = 0; i < A->rows; i++)
    {
        for (j = 0; j < A->cols; j++)
        {
            C->data[i][j] =  A->data[i][j] + B->data[i][j];
        }
    }
}

// Adds two matrices elementwise and returns a new matrix.
Matrix *add_matrix_func(Matrix *A, Matrix *B)
{
    Matrix *C = make_matrix(A->rows, A->cols);
    add_matrix(A, B, C);
    return C;
}

// Performs matrix multiplication and stores the result in the given
// destination matrix (C).
void mult_matrix(Matrix *A, Matrix *B, Matrix *C)
{
    int i, j, k;

    assert(A->rows == B->cols);
    assert(A->rows == C->rows);
    assert(B->cols == C->cols);

    for (i = 0; i < C->rows; i++)
    {
        for (j = 0; j < C->cols; j++)
        {
            for (k = 0; k < A->cols; k++)
            {
                C->data[i][j] += A->data[i][k] * B->data[k][j];
            }
        }
    }
}

// Performs matrix multiplication and returns a new matrix.
Matrix *mult_matrix_func(Matrix *A, Matrix *B)
{
    Matrix *C = make_matrix(A->rows, B->cols);
    mult_matrix(A, B, C);
    return C;
}

double matrix_sum1(Matrix *A)
{
    double total = 0.0;
    int i, j;

    for (i = 0; i < A->rows; i++)
    {
        for (j = 0; j < A->cols; j++)
        {
            total += A->data[i][j];
        }
    }
    return total;
}

double matrix_sum2(Matrix *A)
{
    double total = 0.0;
    int i, j;

    for (j = 0; j < A->cols; j++)
    {
        for (i = 0; i < A->rows; i++)
        {
            total += A->data[i][j];
        }
    }
    return total;
}

// Adds up the rows of A and returns a heap-allocated array of doubles.
double *row_sum(Matrix *A)
{
    double total;
    int i, j;

    double *res = malloc(A->rows * sizeof(double));

    for (i = 0; i < A->rows; i++)
    {
        total = 0.0;
        for (j = 0; j < A->cols; j++)
        {
            total += A->data[i][j];
        }
        res[i] = total;
    }
    return res;
}

/*
   http://en.wikipedia.org/wiki/Magic_square

   A magic square is an arrangement of numbers (usually integers) in a
   square grid, where the numbers in each row, and in each column, and
   the numbers in the forward and backward main diagonals, all add up
   to the same number.

   Write a function called is_magic_square() that takes a matrix and
   returns an int, 1 if the matrix is a magic square, and 0 otherwise.

   Feel free to use row_sum().
*/

//checks if all elements in an array are equal.
int equal_array(double *array, int array_size)
{
    double first_element = array[0];
    int i;
    for (i = 1; i < array_size; ++i)
    {
        if (array[i] != first_element)
        {
            return 0;
        }
    }
    return 1;

}

int is_magic_square(Matrix *A)
{
    assert(A->rows == A->cols);

    int dim = A->rows;
    int i, j;

    double *row_sums = calloc(A->rows, sizeof(double));
    double *col_sums = calloc(A->rows, sizeof(double));

    double diag1_sum = 0.0;
    double diag2_sum = 0.0;
    double elem;

    //Lets only go through the matrix once, to reduce cache misses and augment 
    // the predictive abilities of our MMU
    for (i = 0; i < A->rows; i++)
    {
        for (j = 0; j < A->cols; j++)
        {
            elem = A->data[i][j];

            row_sums[i] += elem;
            col_sums[j] += elem;

            if (i == j)
            {
                //then we are on a forward diagonal
                diag1_sum += elem;
            }
            if (i + j == dim - 1)
            {
                //then we are on a backward diagonal
                diag2_sum += elem;
            }
        }
    }
    
    if (equal_array(row_sums, dim) 
        && equal_array(col_sums, dim) 
        && (diag1_sum == diag2_sum) 
        && (row_sums[0] == col_sums[0])
        && (row_sums[0] == diag1_sum) )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


int main()
{
    // int i;

    Matrix *A = make_matrix(10, 10);
    // consecutive_matrix(A);
    // Matrix *E = make_matrix(10, 10);
    // increment_matrix(E, 10);
    // Matrix *F = add_matrix_func(A, E);
    // printf("F\n");
    // print_matrix(F);

    // Matrix *C = add_matrix_func(A, A);
    // printf("A + A\n");
    // print_matrix(C);

    // Matrix *B = make_matrix(4, 3);
    // increment_matrix(B, 1);
    // printf("B\n");
    // print_matrix(B);

    // Matrix *D = mult_matrix_func(A, B);
    // printf("D\n");
    // print_matrix(D);

    // double sum = matrix_sum1(A);
    // printf("sum = %lf\n", sum);

    // sum = matrix_sum2(A);
    // printf("sum = %lf\n", sum);

    // double *sums = row_sum(A);
    // for (i = 0; i < A->rows; i++)
    // {
    //     printf("row %d\t%lf\n", i, sums[i]);
    // }
    int m = is_magic_square(A);
    printf("%d\n",m);

    return 0;
}
