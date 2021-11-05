#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "input.h"
#include "naive_transpose.h"
#include "parallel_transpose.h"
#include <dlfcn.h>
#include <time.h>



// void print_matrix(const double *matrix, const size_t n_rows, const size_t n_columns)
// {
//     for (size_t i = 0; i < n_rows; i++)
//     {   
//         for (size_t j = 0; j < n_columns; j++)
//             printf("%0.1lf\t", matrix[i * n_columns + j]);
//         printf("\n");
//     }
//     printf("\n");
// }

int main()
{
    FILE *f;
    f = fopen("../test_data.txt", "r");
    if (!f)
        return EXIT_FAILURE;

    double *matrix = NULL;
    size_t n_rows = 0;
    size_t n_columns = 0;

    input(&matrix, &n_rows, &n_columns, f);


    // print_matrix(matrix, n_rows, n_columns);

    
    struct timespec start, finish;

    bool flag_start = clock_gettime(CLOCK_REALTIME, &start);


    bool success = naive_transpose(matrix, &n_rows, &n_columns);
    if (!success)
    {
        free(matrix);
        return EXIT_FAILURE;
    }

    bool flag_finish = clock_gettime(CLOCK_REALTIME, &finish);
    if (!flag_start && !flag_finish)
    {
        size_t spent_time = 1000000000*(finish.tv_sec - start.tv_sec)+(finish.tv_nsec - start.tv_nsec);
        printf("NAIVE TRANSPOSE SPENT TIME: %lu ns \n", spent_time);
    }
    

    

    void *library = dlopen("./libparallel_transpose_lib.so", RTLD_LAZY);
    if (!library)
    {
        free(matrix);
        return EXIT_FAILURE;
    } 
    
    bool(*parallel_transpose)(double *mat, size_t * rows, size_t* columns) = dlsym(library, "parallel_transpose");
    if (!parallel_transpose) 
    {
        free(matrix);
        return EXIT_FAILURE;
    }
    
    
    flag_start = clock_gettime(CLOCK_REALTIME, &start);

    bool test = parallel_transpose(matrix, &n_rows, &n_columns);
    if (!test)
    {
        free(matrix);
        return EXIT_FAILURE;
    }
    flag_finish = clock_gettime(CLOCK_REALTIME, &finish);
    if (!flag_start && !flag_finish)
    {
        size_t spent_time = 1000000000*(finish.tv_sec - start.tv_sec)+(finish.tv_nsec - start.tv_nsec);
        printf("NAIVE TRANSPOSE SPENT TIME: %lu ns \n", spent_time);
    }

    // print_matrix(matrix, n_rows, n_columns);
    free(matrix);
    return EXIT_SUCCESS;
}