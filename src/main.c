#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "input.h"
#include "transpose.h"

#define convert 1000000000



int main()
{
    double *matrix = NULL;
    size_t n_rows = 0;
    size_t n_columns = 0;
    input(&matrix, &n_rows, &n_columns, stdin);
    struct timespec start, finish;
    bool flag_start = clock_gettime(CLOCK_REALTIME, &start);
    bool success = transpose(matrix, &n_rows, &n_columns);
    if (!success)
    {
        free(matrix);
        return EXIT_FAILURE;
    }
    bool flag_finish = clock_gettime(CLOCK_REALTIME, &finish);
    if (!flag_start && !flag_finish)
    {
        size_t spent_time = convert *(finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec);
        printf("SPENT TIME: %lu ns \n", spent_time);
    }
    free(matrix);
    return EXIT_SUCCESS;
}