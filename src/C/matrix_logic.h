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
    if (vec == NULL) {
        printf("vector is NULL\n");
        return;
    }
    for (int i = 0; i < vec->size; i++) {
        vec->values[i] == vec->values[i]? 1 : 0;
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
            mat->weights[i][j] = rand() % 20000 / 10000.0 - 1;
        }
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
struct vector* make_vector_from_txt(char* filename) {
    FILE* opened_file = fopen(filename, "r");
    if (opened_file == NULL) {
        printf("Error: could not open file %s\n", filename);
        return NULL;
    }
    int size;
    fscanf(opened_file, "%d", &size);
    struct vector* result = make_vector(size);
    for (int i = 0; i < size; i++) {
        fscanf(opened_file, "%lf", &result->values[i]);
    }
    fclose(opened_file);
    return result;
}
struct matrix* make_matrix(const unsigned int rows, const unsigned int cols) {
    printf("make matrix\n");
    struct matrix* mat = malloc(sizeof(struct matrix));
    printf("malloc\n");
    mat->rows = rows;
    mat->cols = cols;
    mat->weights = malloc(sizeof(int*) * rows);
    printf("malloc w\n");
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        printf("for loop\n");
            mat->weights[mat_r] = malloc(sizeof(double) * cols);
    }
    printf("done\n");
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
    if (mat == NULL || vec == NULL) {
        printf("Error: matrix weights is NULL\n");
        return NULL;
    }
    if (vec->size != mat->cols) {
        printf("Error: matrix and vector does not match\n");
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
    if (mat == NULL || vec == NULL) {
        printf("Error: matrix weights is NULL\n");
        return NULL;
    }
    if (vec->size != mat->cols) {
        printf("Error: matrix and vector does not match\n");
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
    print_m(mat);
    printf("transpose started\n");
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return NULL;
    }
    struct matrix* result = make_matrix(mat->cols, mat->rows);
    printf("matrix made\n");
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->weights[j][i] = mat->weights[i][j];
        }
    }
    printf("transpose finished\n");
    return result;
}
struct vector* vector_subtract(const struct vector* vec1, const struct vector* vec2) {
    if (vec1->size != vec2->size) {
        printf("vector_subtract cant subtract diferent sized vectors\n");
        return NULL;
    }
    struct vector* result = make_vector(vec1->size);
    for (int i = 0; i < result->size; i++) {
        result->values[i] = vec1->values[i] - vec2->values[i];
    }
    return result;
}



struct AI {
    struct matrix** weights_arr;
    unsigned short layers;
    double learning_rate;
};
struct AI* create_AI(int* layer_arr, unsigned short layers, double learning_rate) {
    if (layer_arr == NULL) {
        printf("Error: layer sizes is NULL\n");
        return NULL;
    }
    if (layers < 2) {
        printf("Error: layers must be greater than 2\n");
        return NULL;
    }
    struct AI* result = malloc(sizeof(struct AI));
    result->layers = layers;
    result->learning_rate = learning_rate;
    result->weights_arr = malloc(sizeof(struct matrix*) * layers-1);
    for (int i = 0; i < layers - 1; i++) {
        result->weights_arr[i] = make_matrix(layer_arr[i + 1], layer_arr[i] + 1);
    }
    return result;
}
void free_AI(struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL\n");
        return;
    }
    for (int i = 0; i < ai->layers - 1; i++) {
        free_matrix(ai->weights_arr[i]);
    }
    free(ai->weights_arr);
    free(ai);
}
void print_AI(struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL\n");
        return;
    }

    for (int i = 0; i < ai->layers - 1; i++) {
        print_m(ai->weights_arr[i]);
    }

}

void randomize_AI_weights(struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL\n");
        return;
    }
    for (int i = 0; i < ai->layers - 1; i++) {
        fill_random_m(ai->weights_arr[i]);
    }
}
struct AI* create_AI_from_txt(char* txt_file) {
    FILE* read_file = fopen(txt_file, "r");
    if (read_file == NULL) {
        printf("Error: could not open file %s\n", txt_file);
        return NULL;
    }
    int layers;
    fscanf(read_file, "%d", &layers);
    int* layers_arr = malloc(sizeof(int) * layers);
    for (int i = 0; i < layers; i++) {
        fscanf(read_file, "%d", &layers_arr[i]);
    }
    double learning_rate;
    for (int i = 0; i < layers; i++) {
        printf("%d ", layers_arr[i]);
    }
    fscanf(read_file, "%lf", &learning_rate);

    struct AI* ai = create_AI(layers_arr, layers, learning_rate);
    free(layers_arr);
    //read body
    for (int i = 0; i < layers - 1; i++) {
        for (int j = 0; j < ai->weights_arr[i]->rows; j++) {
            for (int k = 0; k < ai->weights_arr[i]->cols; k++) {
                fscanf(read_file, "%lf", &(ai->weights_arr[i]->weights[j][k]));
            }
        }
    }

    fclose(read_file);
    printf("ai created from %s\n", txt_file);
    return ai;
}
void save_AI_to_txt(struct AI* ai, char* txt_file) {
    if (ai == NULL) {
        printf("Error: ai is NULL\n");
        return;
    }
    FILE* write_file = fopen(txt_file, "w");
    if (write_file == NULL) {
        printf("Error: could not open file %s\n", txt_file);
        return;
    }
    fprintf(write_file, "%d\n", ai->layers);

    fprintf(write_file, "%d", ai->weights_arr[0]->cols - 1);
    for (int i = 0; i < ai->layers - 1; i++) {
        fprintf(write_file, " %d", ai->weights_arr[i]->rows);
    }
    fprintf(write_file, "\n%f", ai->learning_rate);
    fprintf(write_file, "\n\n");
    for (int i = 0; i < ai->layers - 1; i++) {
        for (int j = 0; j < ai->weights_arr[i]->rows; j++) {
            for (int k = 0; k < ai->weights_arr[i]->cols; k++) {
                fprintf(write_file, "%f ", ai->weights_arr[i]->weights[j][k]);
            }
            fprintf(write_file, "\n");
        }
        fprintf(write_file, "\n");
    }
    fclose(write_file);
    printf("AI saved\n");
}


struct vector* feed_forward(struct vector* input, struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL\n");
        return NULL;
    }
    struct vector* current_layer = multiply_add_bias(ai->weights_arr[0], input);
    activation_function(current_layer);
    for (int i = 1; i < ai->layers - 2; i++) {
        struct vector* new_layer = multiply_add_bias(ai->weights_arr[i], current_layer);

        free_vector(current_layer);
        current_layer = new_layer;
        activation_function(current_layer);
    }
    struct vector* output = multiply(ai->weights_arr[ai->layers - 2], current_layer);
    free_vector(current_layer);
    return output;
}
void train(struct AI* ai, char* training_file, char* expected_file) {
    printf("training\n");
    struct vector* input = make_vector_from_txt(training_file);
    printf("Input\n");
    print_v(input);
    struct vector* output = feed_forward(input, ai);
    printf("Output\n");
    print_v(output);
    struct vector* expected_output = make_vector_from_txt(expected_file);
    printf("Expected\n");
    print_v(expected_output);
    struct vector* output_error = vector_subtract(output, expected_output);
    printf("Error\n");
    print_v(output_error);


    struct vector** hidden_errors = malloc(sizeof(struct vector*) * ai->layers - 2);
    printf("Hidden Errors array made\n");
    struct matrix* weights_t = transpose_m(ai->weights_arr[ai->layers - 1]);
    printf("Weight transposed\n");
    hidden_errors[ai->layers - 3] = multiply(weights_t, output);
    printf("hidden Error calculated\n");
    free_matrix(weights_t);
    for (int i = ai->layers - 4; i >= 0; i--) {
        printf("for loop %d\n", i);
        struct matrix* weights_t = transpose_m(ai->weights_arr[i+1]);
        hidden_errors[i] = multiply(weights_t, hidden_errors[i+1]);
        free_matrix(weights_t);
    }
    printf("gradient decent started\n");



    //gradient descent




    for (int i = 0; i < ai->layers - 2; i++) {
        free_vector(hidden_errors[i]);
    }
    free(hidden_errors);
    free_vector(input);
    free_vector(output);
    free_vector(expected_output);
    free_vector(output_error);
    printf("Training finished\n");
}


#endif //AI_MATRIX_LOGIC_H