#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "parallel_transpose.h"




typedef struct args
{
    double * matrix;
    double * copy_matrix;
    size_t left;
    size_t right;
    size_t n_columns;
    size_t n_rows;
}args;



bool fill_args(args * struct_args, double * matrix, double *copy_matrix, size_t left, size_t right, size_t n_rows, size_t n_columns)
{
    if (!struct_args || !matrix || !copy_matrix || !n_rows || !n_columns)
        return false;
    
    struct_args->matrix = matrix;
    struct_args->copy_matrix = copy_matrix;
    struct_args->left = left;
    struct_args->right = right;
    struct_args->n_columns = n_columns;
    struct_args->n_rows = n_rows;
    return true;

}



void * butch_transpose(void* ptr)
{
    args *butch = (args*)ptr;
    for (size_t idx = butch->left; idx < butch->right; idx++)
    {
        size_t i = idx / butch->n_columns;
        size_t j = idx % butch->n_columns;
        butch->matrix[j * butch->n_rows + i] = butch->copy_matrix[idx];

    }
    return NULL;
}



bool parallel_transpose(double * matrix, size_t *n_rows, size_t *n_columns)
{

    if (!n_rows || !n_columns || !matrix)
        return false;

    size_t size = *n_rows * *n_columns;
    if (size <= 1)
        return false;
        
    double *copy_matrix = (double*)malloc(size * sizeof(double));
    if (!copy_matrix)
        return false;

    memcpy(copy_matrix, matrix, size * sizeof(double));
    if (!copy_matrix)
        return false;

    size_t num_proc = sysconf(_SC_NPROCESSORS_ONLN);
    size_t num_pth = num_proc < size ? num_proc : size;
    pthread_t* pth = (pthread_t*)malloc((num_pth- 1) * sizeof(pthread_t));
    size_t size_butch = size / num_pth;
    args* args_butch = (args*)malloc(num_pth * sizeof(args));
    for (size_t i =  0; i < num_pth - 1; i ++)
    {
        bool fill_flag = fill_args(&args_butch[i], matrix, copy_matrix, size_butch * i,
                                    size_butch * (i +1), *n_rows, *n_columns);
        if (!fill_flag)
        {
            free(pth);
            free(copy_matrix);
            free(args_butch);
            return false;
        }                               
    }
    bool fill_flag = fill_args(&args_butch[num_pth - 1], matrix, copy_matrix, 
                                size_butch * (num_pth - 1),  size, *n_rows, *n_columns);
    if (!fill_flag)
        return false;

    for (size_t i = 0; i < num_pth - 1; i ++)
    { 
        int flag_error = pthread_create(&pth[i], NULL, butch_transpose, (void*)&args_butch[i]);
        if (flag_error)
        {
            free(pth);
            free(copy_matrix);
            free(args_butch);
            return false;
        }
    }

    void * ptr = (void*)&args_butch[num_pth - 1];
    butch_transpose(ptr);

    for (size_t i = 0; i < num_pth - 1; i++)
    {
        int status = pthread_join(pth[i], NULL);
        if (status != 0)
            {
                free(pth);
                free(copy_matrix);
                free(args_butch);
                return(false);
            }
    }

    free(copy_matrix);
    free(pth);
    free(args_butch);
    size_t temp = *n_rows;
    *n_rows = *n_columns;
    *n_columns = temp;
    return true;
}