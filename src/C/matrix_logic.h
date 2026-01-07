#ifndef AI_MATRIX_LOGIC_H
#define AI_MATRIX_LOGIC_H

#include "matrix_locator.h"

#include <stdio.h>
#include <stdlib.h>

double exp(const double x) {
    double result = 1.0;
    double value = 1.0;
    for (int i = 1; i < 8; i++) {// accurate-ness (i can't spell)
        value *= x / i;
        result += value;
    }
    return result;
}



int matrix_count = 0;
int vectors = 0;

struct vector {
    unsigned int size;
    double* values;
};
struct matrix {
    unsigned int rows;
    unsigned int cols;
    double** weights;
};
struct AI {
    struct matrix** weights_arr;
    unsigned short layers;
    double learning_rate;
};



void print_v(struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is NULL\n");
        return;
    }
    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        printf("[%f]\n", vec->values[vec_i]);
    }
    printf("\n");
}
void print_m(struct matrix* mat) {
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


struct vector* make_vector(const unsigned int size) {
    struct vector* vec = malloc(sizeof(struct vector));
    vec->size = size;
    vec->values = malloc(sizeof(double) * size);
    vectors++;
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
    struct matrix* mat = malloc(sizeof(struct matrix));
    mat->rows = rows;
    mat->cols = cols;
    mat->weights = malloc(sizeof(int*) * rows);
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        mat->weights[mat_r] = malloc(sizeof(double) * cols);
    }
    matrix_count++;
    return mat;
}
void free_vector(struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is already NULL\n");
        return;
    }
    free(vec->values);
    free(vec);
    vec = NULL;
    vectors--;
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
    mat = NULL;
    matrix_count--;
}


struct vector* multiply(struct matrix* mat, struct vector* vec) {
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
struct vector* multiply_add_bias(struct matrix* mat, struct vector* vec) {
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
struct vector* multiply_elements_v_v(struct vector* vec1, struct vector* vec2) {
    if (vec1 == NULL || vec2 == NULL) {
        printf("Error: vector is NULL\n");
        return NULL;
    }
    if (vec1->size != vec2->size) {
        printf("Error: vectors does not match\n");
        return NULL;
    }

    struct vector* result = make_vector(vec1->size);
    for (int i = 0; i < vec1->size; i++) {
        result->values[i] = vec1->values[i] * vec2->values[i];
    }
    return result;
}
struct matrix* multiply_v_vt(struct vector* vec1, struct vector* vec2_t, struct AI* ai) {
    if (vec1 == NULL || vec2_t == NULL || ai == NULL) {
        printf("Error: vector is NULL\n");
        return NULL;
    }
    unsigned int row_size = ai->weights_arr[ai->layers - 2]->rows == vec1->size ? vec1->size : vec1->size - 1; // funny stuff because output is the only layer without an added bias
    struct matrix* result = make_matrix(row_size, vec2_t->size);
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < vec2_t->size; j++) {
            result->weights[i][j] = vec1->values[i] * vec2_t->values[j];
        }
    }
    return result;
}
void multiply_scalar_v(struct vector* vec, double scalar) {
    if (vec == NULL) {
        printf("Error: vector is NULL\n");
        return;
    }
    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        vec->values[vec_i] *= scalar;
    }
}
struct matrix* divide_scalar_m(struct matrix* mat, const double scalar) {
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return NULL;
    }
    struct matrix* result = make_matrix(mat->rows, mat->cols);
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            result->weights[mat_r][mat_c] = mat->weights[mat_r][mat_c] / scalar;
        }
    }
    return result;
}

void matrix_fill(struct matrix* mat, double fill_value) {
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return;
    }
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            mat->weights[mat_r][mat_c] = fill_value;
        }
    }
}
void matrix_add(struct matrix* mat1, struct matrix* mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        printf("Error: matrix is NULL\n");
        return;
    }
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        printf("Error: the matrix's don't match\n");
        print_m(mat1);
        print_m(mat2);
        return;
    }
    for (int mat_r = 0; mat_r < mat1->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat1->cols; mat_c++) {
            mat1->weights[mat_r][mat_c] += mat2->weights[mat_r][mat_c];
        }
    }
}
struct matrix* transpose_m(struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is NULL\n");
        return NULL;
    }
    struct matrix* result = make_matrix(mat->cols, mat->rows);
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->weights[j][i] = mat->weights[i][j];
        }
    }
    return result;
}
struct vector* vector_subtract(struct vector* vec1, struct vector* vec2) {
    if (vec1->size != vec2->size) {
        printf("vector_subtract cant subtract different sized vectors\n");
        return NULL;
    }
    struct vector* result = make_vector(vec1->size);
    for (int i = 0; i < result->size; i++) {
        result->values[i] = vec1->values[i] - vec2->values[i];
    }
    return result;
}


void activation_function(struct vector* vec) {
    if (vec == NULL) {
        printf("vector is NULL\n");
        return;
    }
    for (int i = 0; i < vec->size; i++) {
        vec->values[i] == 1.0 / (1.0 + exp(-vec->values[i]));
    }
}
struct vector* derivative_activation_function(struct vector* vec) {
    if (vec == NULL) {
        printf("vector is NULL\n");
        return NULL;
    }
    struct vector* result = make_vector(vec->size);
    for (int i = 0; i < vec->size; i++) {
        result->values[i] = vec->values[i] * (1 - vec->values[i]);
    }
    return result;
}



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
    ai = NULL;
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
void randomize_AI_weights(
    struct AI* ai) {
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


struct matrix* calculate_deltas(struct AI* ai, struct vector* input, struct vector* output, struct vector* output_error) {
    if (ai == NULL || input == NULL || output == NULL || output_error == NULL) {
        printf("Error: somthing is NULL\n");
        return NULL;
    }
    //printf("deriv activate: ");
    struct vector* gradient = derivative_activation_function(output);
    //printf("gradient mult: ");
    struct vector* gradient_mult = multiply_elements_v_v(gradient, output_error);
    multiply_scalar_v(gradient_mult, ai->learning_rate);
    //printf("deltas: ");
    struct matrix* deltas = multiply_v_vt(gradient_mult, input, ai);
    free_vector(gradient);
    free_vector(gradient_mult);
    return deltas;
}

void train(struct AI* ai, char* training_index_file) {
    if (ai == NULL) {
        printf("Error: invalid ai\n");
        return;
    }
    printf("train started\n");

    struct vector** inputs = NULL;
    struct vector** expected_outputs = NULL;
    const int training_size = load_from_locator(inputs, expected_outputs, training_index_file);
    if (training_size <= 0) {
        printf("Error: invalid training size\n");
        return;
    }
    printf("file loaded\n");

    struct matrix** total_deltas = malloc(sizeof(struct matrix*) * ai->layers - 1);
    for (int weight_layer = 0; weight_layer < ai->layers - 2; weight_layer++) {
        struct matrix* total_matrix = make_matrix(ai->weights_arr[weight_layer]->rows, ai->weights_arr[weight_layer]->cols);
        matrix_fill(total_matrix, 0);
    }
    printf("deltas matrix's made\n");

    for (int training_count = 0; training_count < training_size; training_count++) {
        struct vector** hidden = NULL;
        printf("feed forward calling\n");
        struct vector* output = feed_forward(hidden, inputs[training_count], ai);
        printf("forward feed done\n");


        struct vector* output_error = vector_subtract(output, expected_outputs[training_count]);
        struct vector** hidden_errors = back_propogation(output_error, ai);
        printf("back propagation done\n");


        adding_deltas(total_deltas, inputs[training_count],  hidden,  hidden_errors,  output,  output_error, ai);
        printf("adding to total deltas done\n");


        for (int i = 0; i < ai->layers - 2; i++) {
            free_vector(hidden[i]);
            free_vector(hidden_errors[i]);
        }
        free(hidden_errors);
        free(hidden);
        free_vector(output);
        free_vector(output_error);
        printf("matrix count = %d\nvectors = %d\n", matrix_count, vectors);
    }
    printf("delta stuff fully done\n");

    for (int weight_layer = 0; weight_layer < ai->layers - 1; weight_layer++) {
        free(ai->weights_arr[weight_layer]);
        ai->weights_arr[weight_layer] = divide_scalar_m(total_deltas[weight_layer], training_size);
        free_matrix(total_deltas[weight_layer]);
    }
    printf("adjusted weights\n");

    printf("freeing the allocated memory\n");
    for (int i = 0; i < training_size; i++) {
        printf("%d\n", i);
        free_vector(inputs[i]);
        free_vector(expected_outputs[i]);
    }
    free(inputs);
    free(expected_outputs);
    free(total_deltas);
    printf("training complete\n");
}

int load_from_locator(struct vector** o_inputs, struct vector** o_outputs, char* locater_file) {
    FILE* index_file = fopen(locater_file, "r");
    if (index_file == NULL) {
        printf("Error: could not open file %s\n", locater_file);
        return -1;
    }
    int training_size;
    fscanf(index_file, "%d", &training_size);
    if (training_size <= 0) {
        printf("Error: invalid size %d\n", training_size);
        return -1;
    }
    o_inputs = malloc(sizeof(struct vector*) * training_size);
    o_outputs = malloc(sizeof(struct vector*) * training_size);
    char buffer[100];
    char word[50];
    fgets(buffer, sizeof(buffer), index_file);
    for (int i = 0; i < training_size; i++) {
        int position = 0, word_position = 0;
        fgets(buffer, sizeof(buffer), index_file);
        while (buffer[position] != ' ' && position < 50) {
            word[word_position] = buffer[position];
            position++;
            word_position++;
        }
        word[word_position] = '\0';
        o_inputs[i] = make_vector_from_txt(word);
        word_position = 0;
        position++;

        while (buffer[position] != '\n' && position < 100) {
            word[word_position] = buffer[position];
            position++;
            word_position++;
        }
        word[word_position] = '\0';
        o_outputs[i] = make_vector_from_txt(word);
    }
    fclose(index_file);

    return training_size;
}
struct vector* feed_forward(struct vector** o_hidden, struct vector* input, struct AI* ai) {
    printf("feed forward started\n");
    o_hidden = malloc(sizeof(struct vector*) * (ai->layers - 2));
    o_hidden[0] = multiply_add_bias(ai->weights_arr[0], input);
    activation_function(o_hidden[0]);
    for (int i = 1; i < ai->layers - 2; i++) {
        o_hidden[i] = multiply_add_bias(ai->weights_arr[i], o_hidden[i-1]);
        activation_function(o_hidden[i]);
    }
    struct vector* output = multiply(ai->weights_arr[ai->layers - 2], o_hidden[ai->layers - 3]);
    activation_function(output);
    return output;
}
struct vector** back_propogation(struct vector* output_error, struct AI* ai) {
    struct vector** hidden_errors = malloc(sizeof(struct vector*) * (ai->layers - 2));
    struct matrix* weights_t = transpose_m(ai->weights_arr[ai->layers - 2]);
    hidden_errors[ai->layers - 3] = multiply(weights_t, output_error);
    free_matrix(weights_t);
    for (int i = ai->layers - 4; i >= 0; i--) {
        weights_t = transpose_m(ai->weights_arr[i+1]);
        hidden_errors[i] = multiply(weights_t, hidden_errors[i+1]);
        free_matrix(weights_t);
    }
    return hidden_errors;
}
void adding_deltas(struct matrix** total_delta, struct vector* input, struct vector** hidden, struct vector** hidden_error, struct vector* output, struct vector* output_error, struct AI* ai) {
    if (total_delta == NULL || hidden == NULL) {
        printf("Error: total_delta is NULL\n");
        return;
    }
    struct matrix* delta = calculate_deltas(ai, hidden[ai->layers - 3], output, output_error);
    matrix_add(total_delta[ai->layers - 2], delta);
    free_matrix(delta);
    for (int i = ai->layers - 3; i > 0; i--) {
        delta = calculate_deltas(ai, hidden[i - 1], hidden[i], hidden_error[i]);
        matrix_add(total_delta[i], delta);
        free_matrix(delta);
    }
    delta = calculate_deltas(ai, input, hidden[0], hidden_error[0]);
    matrix_add(total_delta[0], delta);
    free_matrix(delta);
}

#endif //AI_MATRIX_LOGIC_H