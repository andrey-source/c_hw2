#include <fstream>
#include "gtest/gtest.h"
#include "stdio.h"
#include "string.h"




extern "C" {
    #include "transpose.h"
}


double matrix_3x2[6] = {1.1, 2.2,
                        3.3, 4.4,
                        5.5, 6.6};

double matrix_2x3_T[6] = {1.1, 3.3, 5.5,
                          2.2, 4.4, 6.6};



TEST(TRANSPOSE, NULL_SIZE) {
    size_t n_rows = 1;
    size_t n_columns = 1;
    size_t size = n_rows * n_columns;
    double *matrix = (double*)malloc(size *sizeof(double));
    ASSERT_NE(matrix, nullptr);
    for (size_t i = 0; i < size; i++)
        matrix[i] = i * 1.1;
    EXPECT_EQ(transpose(matrix, nullptr, &n_columns), false);
    free(matrix);
}

TEST(TRANSPOSE, INCORRECT_SIZE) {
    size_t n_rows = 1;
    size_t n_columns = 1;
    size_t size = n_rows * n_columns;
    double *matrix = (double*)malloc(size *sizeof(double));
    ASSERT_NE(matrix, nullptr);
    for (size_t i = 0; i < size; i++)
        matrix[i] = i * 1.1;
    EXPECT_EQ(transpose(matrix, &n_rows, &n_columns), false);
    free(matrix);
}

TEST(TRANSPOSE, RESIZE) {
    size_t n_rows = 2;
    size_t n_columns = 1;
    size_t size = n_rows * n_columns;
    double *matrix = (double*)malloc(size *sizeof(double));
    ASSERT_NE(matrix, nullptr);
    for (size_t i = 0; i < size; i++)
        matrix[i] = i * 1.1;
    ASSERT_EQ(transpose(matrix, &n_rows, &n_columns), true);
    EXPECT_EQ(n_rows, 1);
    EXPECT_EQ(n_columns, 2);
    free(matrix);
}

TEST(TRANSPOSE, EMPTY_DATA) {
    size_t n_rows = 1;
    size_t n_columns = 2;
    EXPECT_EQ(transpose(NULL, &n_rows, &n_columns), false);
}

TEST(TRANSPOSE, FUNCTIONAL) {
    size_t n_rows = 3;
    size_t n_columns = 2;
    size_t size = n_rows * n_columns;
    double *matrix = (double*)malloc(size * sizeof(double));
    ASSERT_NE(matrix, nullptr);
    for (size_t i = 0; i < size; i++) {
        matrix[i] = matrix_3x2[i];
    }
    ASSERT_EQ(transpose(matrix, &n_rows, &n_columns), true);
    for (size_t i = 0; i < size; i++)
        EXPECT_DOUBLE_EQ(matrix[i], matrix_2x3_T[i]);
    free(matrix);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
