#ifndef AI_MATRIX_LOGIC_H
#define AI_MATRIX_LOGIC_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


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
struct vector* multiply_elements_v_v(struct vector* vec1, const struct vector* vec2) {
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
struct matrix* multiply_v_vt(const struct vector* vec1, const struct vector* vec2_t, struct AI* ai) {
    if (vec1 == NULL || vec2_t == NULL || ai == NULL) {
        printf("Error: vector is NULL\n");
        return NULL;
    }
    unsigned row_size = ai->weights_arr[ai->layers - 2]->rows == vec1->size ? vec1->size : vec1->size - 1; // funny stuf because output is the only layer without a added bias
    struct matrix* result = make_matrix(row_size, vec2_t->size);
    for (int i = 0; i < row_size; i++) {
        for (int j = 0; j < vec2_t->size; j++) {
            result->weights[i][j] = vec1->values[i] * vec2_t->values[j];
        }
    }
    return result;
}
void multiply_scalar_v(const struct vector* vec, const double scalar) {
    if (vec == NULL) {
        printf("Error: vector is NULL\n");
        return;
    }
    for (int vec_i = 0; vec_i < vec->size; vec_i++) {
        vec->values[vec_i] *= scalar;
    }
}
struct matrix* divide_scalar_m(const struct matrix* mat, const double scalar) {
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
void matrix_add(struct matrix* mat1, const struct matrix* mat2) {
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
struct matrix* transpose_m(const struct matrix* mat) {
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
struct vector* vector_subtract(const struct vector* vec1, const struct vector* vec2) {
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
struct vector* derivitive_activation_function(struct vector* vec) {
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


struct matrix* calculate_deltas(struct AI* ai, struct vector* input, struct vector* output, struct vector* output_error) {
    if (ai == NULL || input == NULL || output == NULL || output_error == NULL) {
        printf("Error: somthing is NULL\n");
        return NULL;
    }
    struct vector* gradient = derivitive_activation_function(output);
    struct vector* gradient_mult = multiply_elements_v_v(gradient, output_error);
    multiply_scalar_v(gradient_mult, ai->learning_rate);
    struct matrix* deltas = multiply_v_vt(gradient_mult, input, ai);
    free_vector(gradient);
    free_vector(gradient_mult);
    return deltas;
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

void train_single(struct AI* ai, char* training_file, char* expected_file) {

    struct vector* input = make_vector_from_txt(training_file);
    struct vector** hidden = malloc(sizeof(struct vector*) * ai->layers - 2);
    for (int i = 0; i < ai->layers - 2; i++) {
        hidden[i] = malloc(sizeof(struct vector));
    }

    hidden[0] = multiply_add_bias(ai->weights_arr[0], input);
    activation_function(hidden[0]);
    for (int i = 1; i < ai->layers - 2; i++) {
        hidden[i] = multiply_add_bias(ai->weights_arr[i], hidden[i-1]);
        activation_function(hidden[i]);
    }
    struct vector* output = multiply(ai->weights_arr[ai->layers - 2], hidden[ai->layers - 3]);
    activation_function(output);
    struct vector* expected_output = make_vector_from_txt(expected_file);
    struct vector* output_error = vector_subtract(output, expected_output);



    struct vector** hidden_errors = malloc(sizeof(struct vector*) * ai->layers - 2);
    struct matrix* weights_t = transpose_m(ai->weights_arr[ai->layers - 2]);
    hidden_errors[ai->layers - 3] = multiply(weights_t, output);
    free_matrix(weights_t);
    for (int i = ai->layers - 4; i >= 0; i--) {
        struct matrix* weights_t = transpose_m(ai->weights_arr[i+1]);
        hidden_errors[i] = multiply(weights_t, hidden_errors[i+1]);
        free_matrix(weights_t);
    }


    struct matrix* deltas = calculate_deltas(ai, hidden[ai->layers - 3], output, output_error);
    matrix_add(ai->weights_arr[ai->layers - 2], deltas);
    free_matrix(deltas);
    for (int i = ai->layers - 3; i > 0; i--) {
        deltas = calculate_deltas(ai, hidden[i - 1], hidden[i], hidden_errors[i]);
        matrix_add(ai->weights_arr[0], deltas);
        free_matrix(deltas);
    }
    deltas = calculate_deltas(ai, input, hidden[0], hidden_errors[0]);
    matrix_add(ai->weights_arr[0], deltas);
    free_matrix(deltas);




    free_vector(input);
    for (int i = 0; i < ai->layers - 2; i++) {
        free_vector(hidden_errors[i]);
        free_vector(hidden[i]);
    }
    free(hidden);
    free(hidden_errors);
    free_vector(output);
    free_vector(expected_output);
    free_vector(output_error);
}

void train(struct AI* ai, char* training_index_file) {
    printf("train2 started\n");
    FILE* index_file = fopen(training_index_file, "r");
    if (index_file == NULL) {
        printf("Error: could not open file %s\n", training_index_file);
        return;
    }
    int training_size;
    fscanf(index_file, "%d", &training_size);
    if (training_size <= 0) {
        printf("Error: invalid size %d\n", training_size);
        return;
    }
    struct vector** inputs = malloc(sizeof(struct vector*) * training_size);
    struct vector** expected_outputs = malloc(sizeof(struct vector*) * training_size);

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
        inputs[i] = make_vector_from_txt(word);
        word_position = 0;
        position++;

        while (buffer[position] != '\n' && position < 100) {
            word[word_position] = buffer[position];
            position++;
            word_position++;
        }
        word[word_position] = '\0';
        expected_outputs[i] = make_vector_from_txt(word);
    }
    fclose(index_file);
    printf("read file into inputs[] and expected_outputs[]\n");


    struct matrix*** deltas = malloc(sizeof(struct matrix**) * training_size); //an array of arrays of matrix pointers, god has no eyes
    for (int i = 0; i < training_size; i++) {
        deltas[i] = malloc(sizeof(struct matrix*) * (ai->layers - 2));
    }// making the arrays of matrix pointers
    printf("deltas *** made\n");


    for (int training_count = 0; training_count < training_size; training_count++) {
        struct vector** hidden = malloc(sizeof(struct vector*) * (ai->layers - 2));

        hidden[0] = multiply_add_bias(ai->weights_arr[0], inputs[training_count]);
        activation_function(hidden[0]);
        for (int i = 1; i < ai->layers - 2; i++) {
            hidden[i] = multiply_add_bias(ai->weights_arr[i], hidden[i-1]);
            activation_function(hidden[i]);
        }
        struct vector* output = multiply(ai->weights_arr[ai->layers - 2], hidden[ai->layers - 3]);
        activation_function(output);
        printf("forward feed done\n");

        struct vector* output_error = vector_subtract(output, expected_outputs[training_count]);
        struct vector** hidden_errors = malloc(sizeof(struct vector*) * (ai->layers - 2));
        struct matrix* weights_t = transpose_m(ai->weights_arr[ai->layers - 2]);
        hidden_errors[ai->layers - 3] = multiply(weights_t, output);
        free_matrix(weights_t);
        for (int i = ai->layers - 4; i >= 0; i--) {
            struct matrix* weights_t = transpose_m(ai->weights_arr[i+1]);
            hidden_errors[i] = multiply(weights_t, hidden_errors[i+1]);
            free_matrix(weights_t);
        }
        printf("backward feed done\n");

        deltas[training_count][ai->layers - 2] = calculate_deltas(ai, hidden[ai->layers - 3], output, output_error);;
        for (int i = ai->layers - 3; i > 0; i--) {
            deltas[training_count][i] = calculate_deltas(ai, hidden[i - 1], hidden[i], hidden_errors[i]);
        }
        deltas[training_count][0] = calculate_deltas(ai, inputs[training_count], hidden[0], hidden_errors[0]);
        printf("calculated deltas\n");
    }


    printf("adjusting weights\n");
    for (int weight_layer = 0; weight_layer < ai->layers - 2; weight_layer++) {
        struct matrix* total_matrix = make_matrix(ai->weights_arr[weight_layer]->rows, ai->weights_arr[weight_layer]->cols);
        matrix_fill(total_matrix, 0);
        for (int training_count = 0; training_count < training_size; training_count++) {
            matrix_add(total_matrix, deltas[training_count][weight_layer]);
            free_matrix(deltas[training_count][weight_layer]);
        }
        free_matrix(ai->weights_arr[weight_layer]);
        ai->weights_arr[weight_layer] = divide_scalar_m(total_matrix, training_size);
        free_matrix(total_matrix);
    }//adjusting the weights


    printf("freeing the allocated memory\n");
    for (int i = 0; i < training_size; i++) {
        printf("%d\n", i);
        free(deltas[i]);
        free_vector(inputs[i]);
        free_vector(expected_outputs[i]);
    }
    printf("free pt2\n");
    free(deltas);
    free(inputs);
    free(expected_outputs);
}


#endif //AI_MATRIX_LOGIC_H