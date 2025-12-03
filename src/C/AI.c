#include <stdio.h>
#include <stdbool.h>
#include "matrix_logic.h"

#define one_bias 1
#define input_size 2
#define hidden_size 5
#define output_size 1


struct vector* forward_feed(struct vector* input, struct matrix* mat1, struct matrix* mat2) {
    struct vector* hidden = multiply_add_bias(mat1, input);
    activation_function(hidden);
    struct vector* output = multiply(mat2, hidden);
    activation_function(output);
    free_vector(hidden);
    return output;
}

void back_track(struct matrix* mat1, struct matrix* mat2, struct vector* output, struct vector* expected_output) {

    double error = output->values[0] - expected_output->values[0];
    printf("error = %f\n\n", error);
    struct vector* error_hidden = make_vector(hidden_size);



    free_vector(error_hidden);
}


int main(void) {
    struct vector* input = make_vector(input_size + one_bias);
    fill_random_v(input);
    input->values[input_size] = 1;
    struct matrix* mat1 = make_matrix(hidden_size,input_size + one_bias);
    fill_random_m(mat1);
    struct matrix* mat2 = make_matrix(output_size,hidden_size + one_bias);
    fill_random_m(mat2);

    double learning_rate = 0.05;

    for (int i = 0; i < 3; i++) {//train XOR
        input->values[0] = rand()%2 * 2 - 1; //training values
        input->values[1] = rand()%2 * 2 - 1;
        printf("input = %f, %f\n", input->values[0], input->values[1]);
        struct vector* output = forward_feed(input, mat1, mat2);
        printf("output = %f\n", output->values[0]);
        struct vector* expected_output = make_vector(output_size);

        back_track(mat1, mat2, output, expected_output);

        free_vector(expected_output);
        free_vector(output);
    }


    int player_input = 0;
    printf("now you test it, 69 is exit value\n");
    while (true) {
        scanf("%d", &player_input);
        if (player_input == 69) {break;}
        input->values[0] = (bool)player_input / 10;
        input->values[1] = (bool)player_input % 10;
        struct vector* output = forward_feed(input, mat1, mat2);
        printf("output = ");
        print_v(output);
        free_vector(output);
    }



    free_vector(input);
    free_matrix(mat1);
    free_matrix(mat2);
}
