#ifndef AI_MATRIX_LOGIC_H
#define AI_MATRIX_LOGIC_H

#include "matrix_locator.h"

#include <stdio.h>
#include <stdlib.h>

void h(struct vector* input) {
    printf("\n________________________\n");
    //print_v(input);
    //print_AI(ai);
}


int matrix_count = 0; //debug stuff
int vectors = 0; //helps check for memory leaking
_Bool warnings = 1;


//structures used
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



//activation functions
inline double exp(const double x) {
    double result = 1.0;
    double value = 1.0;
    for (int i = 1; i < 8; i++) {// accurate-ness (i can't spell)
        value *= x / i;
        result += value;
    }
    return result;
} //formula e^x used in sigmoid [activation function]
void activation_function(const struct vector* vec) {
    if (vec == NULL) {
        printf("vector is NULL [activation function]\n");
        return;
    }
    if (vec->values == NULL) {
        printf("vector values is NULL [activation function]\n");
        return;
    }

    for (int i = 0; i < vec->size; i++) {
        vec->values[i] == 1.0 / (1.0 + exp(-vec->values[i]));
    }
}
struct vector* derivative_activation_function(const struct vector* vec) {
    if (vec == NULL) {
        printf("vector is NULL [derivative activation function]\n");
        return NULL;
    }
    if (vec->values == NULL) {
        printf("vector values is NULL [derivative activation function]\n");
        return NULL;
    }

    struct vector* result = make_vector(vec->size);
    for (int i = 0; i < vec->size; i++) {
        result->values[i] = vec->values[i] * (1 - vec->values[i]);
    }
    return result;
}



//vector actions
struct vector* make_vector(const unsigned int size) {
    if (size == 0) {
        printf("Error: vector size is 0 [make vector]\n");
        return NULL;
    }
    if (warnings) {
        printf("Warning: created vector size is %d\n", size);
    }

    struct vector* vec = malloc(sizeof(struct vector));
    if (vec == NULL) {
        printf("Error: vector malloc failed [make vector]\n");
        return NULL;
    }
    vec->size = size;
    vec->values = malloc(sizeof(double) * size);
    if (vec->values == NULL) {
        printf("Error: vector values malloc failed [make vector]\n");
        return NULL;
    }

    vectors++;
    return vec;
}
struct vector* make_vector_from_txt(char* filename) {
    FILE* opened_file = fopen(filename, "r");
    if (opened_file == NULL) {
        printf("Error: could not open file %s [make vector from txt]\n", filename);
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
void free_vector(struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is already NULL [free vector]\n");
        return;
    }

    free(vec->values);
    free(vec);
    vec = NULL;
    vectors--;
}
void print_v(const struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is NULL [print vector]\n");
        return;
    }
    if (vec->values == NULL) {
        printf("vector values is NULL [print vector]\n");
        return;
    }

    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        printf("[%f]\n", vec->values[vec_i]);
    }
    printf("\n");
}
void fill_random_v(const struct vector* vec) {
    if (vec == NULL) {
        printf("Error: vector is NULL [fill vector random]\n");
        return;
    }
    if (vec->values == NULL) {
        printf("Error: vector values is NULL [fill vector random]\n");
        return;
    }

    for (int i = 0; i < vec->size; i++) {
        vec->values[i] = rand() % 2000 / 1000.0 - 1;
    }
}
void multiply_scalar_v(const struct vector* vec, const double scalar) {
    if (vec == NULL) {
        printf("Error: vector is NULL [multiply vector by scalar]\n");
        return;
    }
    if (vec->values == NULL) {
        printf("Error: vector values is NULL [multiply vector by scalar]\n");
        return;
    }

    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        vec->values[vec_i] *= scalar;
    }
}



//matrix actions
struct matrix* make_matrix(const unsigned int rows, const unsigned int cols) {
    if (rows == 0 || cols == 0) {
        printf("Error: matrix rows or columns is 0 [make matrix]\n");
        return 0;
    }
    if (warnings) {
        printf("Warning: created matrix has dimensions %d %d\n", rows, cols);
    }

    struct matrix* mat = malloc(sizeof(struct matrix));
    if (mat == NULL) {
        printf("Error: matrix malloc failed [make matrix]\n");
        return NULL;
    }

    mat->rows = rows;
    mat->cols = cols;
    mat->weights = malloc(sizeof(int*) * rows);
    if (mat->weights == NULL) {
        printf("Error: matrix weights malloc failed [make matrix]\n");
        return NULL;
    }

    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        mat->weights[mat_r] = malloc(sizeof(double) * cols);
        if (mat->weights[mat_r] == NULL) {
            printf("Error: matrix internal weights  malloc failed [make matrix]\n");
            return NULL;
        }
    }
    matrix_count++;
    return mat;
}
void free_matrix(struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is already NULL [free matrix]\n");
        return;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [free matrix]\n");
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
void print_m(const struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is NULL [print matrix]\n");
        return;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [print matrix]\n");
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
void fill_random_m(const struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is NULL [fill matrix random]\n");
        return;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [fill matrix random]\n");
        return;
    }

    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->weights[i][j] = rand() % 20000 / 10000.0 - 1;
        }
    }
}
void matrix_fill(const struct matrix* mat, const double fill_value) {
    if (mat == NULL) {
        printf("Error: matrix is NULL [matrix fill]\n");
        return;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [matrix fill]\n");
        return;
    }

    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            mat->weights[mat_r][mat_c] = fill_value;
        }
    }
}
struct matrix* divide_scalar_m(const struct matrix* mat, const double scalar) {
    if (mat == NULL) {
        printf("Error: matrix is NULL [divide matrix by scalar]\n");
        return NULL;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [divide matrix by scalar]\n");
        return NULL;
    }

    struct matrix* result = make_matrix(mat->rows, mat->cols);
    if (result == NULL) {
        printf("Error: created matrix is NULL [divide matrix by scalar]\n");
        return NULL;
    }
    for (int mat_r = 0; mat_r < mat->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat->cols; mat_c++) {
            result->weights[mat_r][mat_c] = mat->weights[mat_r][mat_c] / scalar;
        }
    }
    return result;
}
struct matrix* transpose_m(const struct matrix* mat) {
    if (mat == NULL) {
        printf("Error: matrix is NULL [transpose matrix]\n");
        return NULL;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [transpose matrix]\n");
        return NULL;
    }

    struct matrix* result = make_matrix(mat->cols, mat->rows);
    if (result == NULL) {
        printf("Error: created matrix is NULL [transpose matrix]\n");
        return NULL;
    }
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            result->weights[j][i] = mat->weights[i][j];
        }
    }
    return result;
}



//mixed actions
struct vector* multiply(const struct matrix* mat, const struct vector* vec) {
    if (mat == NULL) {
        printf("Error: matrix is NULL [multiply]\n");
        return NULL;
    }
    if (vec == NULL) {
        printf("Error: vector is NULL [multiply]\n");
        return NULL;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [multiply]\n");
        return NULL;
    }
    if (vec->values == NULL) {
        printf("Error: vector values is NULL [multiply]\n");
        return NULL;
    }
    if (vec->size != mat->cols) {
        printf("Error: matrix and vector does not match [multiply]\n");
        return NULL;
    }

    struct vector* result = make_vector(mat->rows);
    if (result == NULL) {
        printf("Error: created vector is NULL [multiply]\n");
        return NULL;
    }
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
    if (mat == NULL) {
        printf("Error: matrix is NULL [multiply + bias]\n");
        return NULL;
    }
    if (vec == NULL) {
        printf("Error: vector is NULL [multiply + bias]\n");
        return NULL;
    }
    if (mat->weights == NULL) {
        printf("Error: matrix weights is NULL [multiply + bias]\n");
        return NULL;
    }
    if (vec->values == NULL) {
        printf("Error: vector values is NULL [multiply + bias]\n");
        return NULL;
    }
    if (vec->size != mat->cols) {
        printf("Error: matrix and vector does not match [multiply + bias]\n");
        return NULL;
    }

    struct vector* result = make_vector(mat->rows + 1);
    if (result == NULL) {
        printf("Error: created vector is NULL [multiply + bias]\n");
        return NULL;
    }

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
struct vector* multiply_elements_v_v(const struct vector* vec1, const struct vector* vec2) {
    if (vec1 == NULL || vec2 == NULL) {
        printf("Error: vector is NULL [multiply vectors elementwise]\n");
        return NULL;
    }
    if (vec1->size != vec2->size) {
        printf("Error: vectors does not match [multiply vectors elementwise]\n");
        return NULL;
    }

    struct vector* result = make_vector(vec1->size);
    if (result == NULL) {
        printf("Error: created vector is NULL [multiply vectors elementwise]\n");
        return NULL;
    }
    for (int i = 0; i < vec1->size; i++) {
        result->values[i] = vec1->values[i] * vec2->values[i];
    }
    return result;
}
struct matrix* multiply_v_vt(const struct vector* vec1, const struct vector* vec2_t, const struct AI* ai) {
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
void matrix_add(const struct matrix* mat1, const struct matrix* mat2) {
    if (mat1 == NULL || mat2 == NULL) {
        printf("Error: matrix is NULL [matrix add]\n");
        return;
    }
    if (mat1->rows != mat2->rows || mat1->cols != mat2->cols) {
        printf("Error: the matrix's don't match [matrix add]\n");
        return;
    }

    for (int mat_r = 0; mat_r < mat1->rows; mat_r++) {
        for (int mat_c = 0; mat_c < mat1->cols; mat_c++) {
            mat1->weights[mat_r][mat_c] += mat2->weights[mat_r][mat_c];
        }
    }
}
struct vector* vector_subtract(const struct vector* vec1, const struct vector* vec2) {
    if (vec1 == NULL || vec2 == NULL) {
        printf("Error: vector is NULL [vector subtract]\n");
        return NULL;
    }
    if (vec1->size != vec2->size) {
        printf("vectors are sized different [vector subtract]\n");
        return NULL;
    }

    struct vector* result = make_vector(vec1->size);
    if (result == NULL) {
        printf("Error: created vector is NULL [vector subtract]\n");
        return NULL;
    }
    for (int i = 0; i < result->size; i++) {
        result->values[i] = vec1->values[i] - vec2->values[i];
    }
    return result;
}



//AI actions
struct AI* create_AI(const int* layer_arr, const unsigned short layers, const double learning_rate) {
    if (layer_arr == NULL) {
        printf("Error: layer sizes is NULL [create AI]\n");
        return NULL;
    }
    if (layers < 2) {
        printf("Error: layers must be greater than 2 [create AI]\n");
        return NULL;
    } //might be able to make this < 1

    struct AI* result = malloc(sizeof(struct AI));
    if (result == NULL) {
        printf("Error: created AI is NULL [create AI]\n");
        return NULL;
    }
    result->layers = layers;
    result->learning_rate = learning_rate;
    result->weights_arr = malloc(sizeof(struct matrix*) * layers-1);
    if (result->weights_arr == NULL) {
        printf("Error: failed to create weights array\n");
        return NULL;
    }
    for (int i = 0; i < layers - 1; i++) {
        result->weights_arr[i] = make_matrix(layer_arr[i + 1], layer_arr[i] + 1);
    }
    return result;
}
struct AI* create_AI_from_txt(char* txt_file) {
    FILE* read_file = fopen(txt_file, "r");
    if (read_file == NULL) {
        printf("Error: could not open file %s [create AI from txt]\n", txt_file);
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
void free_AI(struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL [free AI]\n");
        return;
    }
    if (ai->weights_arr == NULL) {
        printf("AI weights_arr is NULL [free AI weights_arr]\n");
        return;
    }
    for (int i = 0; i < ai->layers - 1; i++) {
        free_matrix(ai->weights_arr[i]);
    }
    free(ai->weights_arr);
    free(ai);
    ai = NULL;
}
void save_AI_to_txt(const struct AI* ai, char* txt_file) {
    if (ai == NULL) {
        printf("Error: ai is NULL [save AI to txt]\n");
        return;
    }
    if (ai->weights_arr == NULL) {
        printf("AI weights_arr is NULL [save AI weights_arr]\n");
        return;
    }
    FILE* write_file = fopen(txt_file, "w");
    if (write_file == NULL) {
        printf("Error: could not open file %s [save AI to txt]\n", txt_file);
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
void print_AI(const struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL [print AI]\n");
        return;
    }
    if (ai->weights_arr == NULL) {
        printf("Error: AI weights_arr is NULL [print AI weights_arr]\n");
        return;
    }

    for (int i = 0; i < ai->layers - 1; i++) {
        print_m(ai->weights_arr[i]);
    }
}
void randomize_AI_weights(const struct AI* ai) {
    if (ai == NULL) {
        printf("Error: AI is NULL [randomise AI weights]\n");
        return;
    }
    if (ai->weights_arr == NULL) {
        printf("Error: AI weights_arr is NULL [randomize AI weights_arr]\n");
        return;
    }

    for (int i = 0; i < ai->layers - 1; i++) {
        fill_random_m(ai->weights_arr[i]);
    }
}




//training functions
void train(const struct AI* ai, char* training_index_file) {
    if (ai == NULL) {
        printf("Error: ai is NULL [train]\n");
        return;
    }
    if (ai->weights_arr == NULL) {
        printf("Error: AI weights_arr is NULL [train weights_arr]\n");
        return;
    }
    if (training_index_file == NULL) {
        printf("Error: training index_file is NULL [train]\n");
        return;
    }
    printf("train started\n");

    struct vector** inputs = NULL;
    struct vector** expected_outputs = NULL;
    const int training_size = load_from_locator(inputs, expected_outputs, training_index_file);
    if (training_size <= 0) {
        printf("Error: invalid training size [train]\n");
        return;
    }
    printf("file loaded\n");

    struct matrix** total_deltas = malloc(sizeof(struct matrix*) * ai->layers - 1);
    for (int weight_layer = 0; weight_layer < ai->layers - 1; weight_layer++) {
        total_deltas[weight_layer] = make_matrix(ai->weights_arr[weight_layer]->rows, ai->weights_arr[weight_layer]->cols);
        matrix_fill(total_deltas[weight_layer], 0);
    }
    printf("deltas matrix's made\n");

    for (int training_count = 0; training_count < training_size; training_count++) {
        struct vector** hidden = NULL;
        printf("a = %d\n", inputs);
        h(inputs[training_count]);
        //inputs[training_count] argument causing an issue
        struct vector* output = feed_forward(hidden, inputs[training_count], ai);
        printf("forward feed done\n");


        struct vector* output_error = vector_subtract(output, expected_outputs[training_count]);
        struct vector** hidden_errors = back_propagation(output_error, ai);
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
    if (o_inputs == NULL) {
        printf("Error: o_inputs is NULL [load from locator]\n");
        o_inputs = (void*)1;
    }
    if (locater_file == NULL) {
        printf("Error: locater_file is NULL [load from locator]\n");
        return -1;
    }
    FILE* index_file = fopen(locater_file, "r");
    if (index_file == NULL) {
        printf("Error: could not open file %s [load from locator]\n", locater_file);
        return -1;
    }
    int training_size;
    fscanf(index_file, "%d", &training_size);
    if (training_size <= 0) {
        printf("Error: invalid size %d [load from locator]\n", training_size);
        return -1;
    }
    o_inputs = malloc(sizeof(struct vector*) * training_size);
    o_outputs = malloc(sizeof(struct vector*) * training_size);
    if (o_inputs == NULL || o_outputs == NULL) {
        printf("Error: could not allocate memory for vectors [load from locator]\n");
        free(o_inputs);
        free(o_outputs);
        return -1;
    }
    printf("b = %d, %d", o_inputs, o_outputs);

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
    printf("c = %d, %d", o_inputs, o_outputs);
    return training_size;
}
struct vector* feed_forward(struct vector** o_hidden, const struct vector* input, const struct AI* ai) {
    printf("fjdskf");
    if (ai == NULL) {
        printf("Error: ai is NULL [feed_forward]\n");
        return NULL;
    }
    if (ai->weights_arr == NULL) {
        printf("Error: weights_arr is NULL [feed_forward]\n");
        return NULL;
    }
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
struct vector** back_propagation(const struct vector* output_error, const struct AI* ai) {
    if (ai == NULL) {
        printf("Error: ai is NULL [back_propagation]\n");
        return NULL;
    }
    if (ai->weights_arr == NULL) {
        printf("Error: weights_arr is NULL [back_propagation]\n");
        return NULL;
    }
    if (output_error == NULL) {
        printf("Error: output_error is NULL [back_propagation]\n");
        return NULL;
    }

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
struct matrix* calculate_deltas(const struct AI* ai, const struct vector* input, const struct vector* output, const struct vector* output_error) {
    if (ai == NULL || input == NULL || output == NULL || output_error == NULL) {
        printf("Error: somthing is NULL [calculate deltas]\n");
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
void adding_deltas(struct matrix** total_delta, const struct vector* input, struct vector** hidden, struct vector** hidden_error, const struct vector* output, const struct vector* output_error, const struct AI* ai) {
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