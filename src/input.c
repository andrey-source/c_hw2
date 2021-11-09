#include <stdlib.h>
#include "input.h"



bool input_size(size_t *n_rows, size_t *n_columns, FILE *stream) {
    if (stream == stdin) {
        printf("Enter the number of rows\n");
    }
    if (!fscanf(stream, "%lu", n_rows) || *n_rows > MAX_SIZE) {
        return false;
    }
    if (stream == stdin) {
        printf("Enter the number of columns\n");
    }
    if (!fscanf(stream, "%lu", n_columns) || *n_columns > MAX_SIZE) {
        return false;
    }
    return true;
}

bool fill_matrix(double *matrix, size_t n_rows, size_t n_columns, FILE *stream) {
    if (n_rows * n_columns < 1 || n_rows > MAX_SIZE || n_columns > MAX_SIZE) {
        return false;
    }
    for (size_t i = 0; i < n_rows; i++) {
        for (size_t j = 0; j < n_columns; j++) {
            double input_num;
            if (fscanf(stream, "%lf", &input_num) != 1) {
                return false;
            }
            matrix[n_columns * i + j] = input_num;
        }
    }
    return true;
}

bool input(double **matrix, size_t *n_rows, size_t *n_columns, FILE *stream) {
    if (!matrix || !input_size(n_rows, n_columns, stream)) {
        return false;
    }
    double *new_matrix = (double *)malloc(*n_rows * *n_columns * sizeof(double));
    if (!new_matrix) {
        return false;
    }
    if (stream == stdin) {
        printf("Enter %lu matrix elements\n", *n_rows * *n_columns);
    }
    if (!fill_matrix(new_matrix, *n_rows, *n_columns, stream)) {
        free(new_matrix);
        return false;
    }
    *matrix = new_matrix;
    return true;
}
