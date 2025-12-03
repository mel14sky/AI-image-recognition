#ifndef AI_MATRIX_LOGIC_H
#define AI_MATRIX_LOGIC_H

#include <stdio.h>
#include <stdlib.h>

struct vector {
    unsigned int size;
    double* values;
};
struct matrix {
    unsigned int rows;
    unsigned int cols;
    double** weights;
};

void activation_function(struct vector* vec) {
    for (int i = 0; i < vec->size; i++) {
        vec->values[i] = vec->values[i] > 0 ? 1 : -1;
    }
}


void fill_random_v(struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is NULL\n");
        return;
    }
    for (int i = 0; i < vec->size; i++) {
        vec->values[i] = rand() % 2000 / 1000.0 - 1;
    }
}
void fill_random_m(struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->weights[i][j] = rand() % 2000 / 1000.0 - 1;
        }
    }
}
void fill_from_txt(struct matrix* mat, char* filename) {
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return;
    }
    FILE* matrix_read = fopen(filename, "r");
    if (matrix_read == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }

    //read matrix from txt file
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL\n");
        return;
    }
    for (int rows = 0; rows < mat->rows; rows++) {
        if (mat->weights[rows] == NULL) {
            printf("Error: matrix weights is NULL\n");
            return;
        }
        for (int cols = 0; cols < mat->cols; cols++) {
            int num;
            fscanf(matrix_read, "%d", &num);
            mat->weights[rows][cols] = num;
        }
    }

    fclose(matrix_read);
}
void save_to_txt(const struct matrix* mat, char* filename) {
    if (mat == NULL || mat->weights == NULL) {
        printf("Error: matrix is NULL\n");
        return;
    }
    FILE* matrix_write_to = fopen(filename, "w");
    if (matrix_write_to == NULL) {
        printf("Error: could not open file %s\n", filename);
        return;
    }
    for (int i = 0; i < mat->rows; i++) {
        if (mat->weights[i] == NULL) {
            printf("Error: weights not found\n");
            return;
        }
        for (int j = 0; j < mat->cols; j++) {
            fprintf(matrix_write_to, "%d ", mat->weights[i][j]);
        }
        fprintf(matrix_write_to, "\n");
    }
}

void print_v(const struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is NULL\n");
        return;
    }
    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        printf("[%f]\n", vec->values[vec_i]);
    }
    printf("\n");
}
void print_m(const struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return;
    }
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        printf("[");
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            printf("%f", mat->weights[mat_r][mat_c]);
            if (mat_c != mat->cols - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
    printf("\n");
}

struct vector* make_vector(const unsigned int size) {
    struct vector* vec = malloc(sizeof(struct vector));
    vec->size = size;
    vec->values = malloc(sizeof(double) * size);
    return vec;
}
struct matrix* make_matrix(const unsigned int rows, const unsigned int cols) {
    struct matrix* mat = malloc(sizeof(struct matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->weights = malloc(sizeof(int*) * rows);
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
            mat->weights[mat_r] = malloc(sizeof(double) * cols);
    }
    return mat;
}
void free_vector(struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is already NULL\n");
        return;
    }
    free(vec->values);
    free(vec);
}
void free_matrix(struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is already NULL\n");
        return;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL\n");
        return;
    }
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        if (mat->weights[mat_r] != NULL) {
            free(mat->weights[mat_r]);
        }
    }
    free(mat->weights);
    free(mat);
}

struct vector* multiply(const struct matrix* mat, const struct vector* vec) {
    if (vec->size != mat->cols) {
        printf("Error: matrix and vector does not match\n");
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL\n");
        return NULL;
    }
    struct vector* result = make_vector(mat->rows);
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        double sum = 0;
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {

            sum += mat->weights[mat_r][mat_c] * vec->values[mat_c];
        }
        result->values[mat_r] = sum;
    }
    return result;
}
struct vector* multiply_add_bias(const struct matrix* mat, const struct vector* vec) {
    if (vec->size != mat->cols) {
        printf("Error: matrix and vector does not match\n");
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL\n");
        return NULL;
    }
    struct vector* result = make_vector(mat->rows + 1);
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        double sum = 0;
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            sum += mat->weights[mat_r][mat_c] * vec->values[mat_c];
        }
        result->values[mat_r] = sum;
    }
    result->values[mat->rows] = 1;
    return result;
}
struct vector* multiply_scalar_v(const struct vector* vec, const double scalar) {
    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        vec->values[vec_i] *= scalar;
    }
}
struct matrix* matrix_add(const struct matrix* mat1, const struct matrix* mat2) {
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        printf("Error: the matrix does not match\n");
    }
    for (int mat_r = 0; mat_r < mat1->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat1->cols; mat_c++) {
            mat1->weights[mat_r][mat_c] += mat2->weights[mat_r][mat_c];
        }
    }
}
struct matrix* transpose_m(const struct matrix* mat) {
    struct matrix* result = make_matrix(mat->cols, mat->rows);
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            result->weights[mat_c][mat_r] = mat->weights[mat_r][mat_c];
        }
    }
    return result;
}


#endif //AI_MATRIX_LOGIC_H