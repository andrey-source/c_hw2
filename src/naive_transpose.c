#include <string.h>
#include "transpose.h"



bool transpose(double * matrix, size_t *n_rows, size_t *n_columns) {
    if (!n_rows || !n_columns || !matrix)
        return false;

    size_t size = *n_rows * *n_columns;
    if (size <= 1)
        return false;

    double *copy_matrix = (double*)malloc(size * sizeof(double));
    if (!copy_matrix)
        return false;

    memcpy(copy_matrix, matrix, size * sizeof(double));
    for (size_t i = 0; i < *n_rows; i++) {
        for (size_t j = 0; j < *n_columns; j++) {
           matrix[j * *n_rows + i] = copy_matrix[i * *n_columns + j];
        }
    }
    free(copy_matrix);
    size_t temp = *n_rows;
    *n_rows = *n_columns;
    *n_columns = temp;
    return true;
}
