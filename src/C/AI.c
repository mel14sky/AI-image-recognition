#include <stdio.h>
#include "matrix_logic.h"



int main(int argc, char** argv) {
    if (argc != 3) {
        printf("argument count not correct\n");
        return -1;
    }
    struct AI* ai = make_AI_from_txt(argv[1]);
    struct vector* input = make_vector_from_txt(argv[2]);

    struct vector* hidden = multiply_add_bias(ai->weights_arr[0], input);
    activation_function(hidden);
    for (int i = 1; i < ai->layers - 2; i++) {
        struct vector* temp = multiply_add_bias(ai->weights_arr[i], hidden);
        free_vector(hidden);
        hidden = temp;
        activation_function(hidden);
    }
    struct vector* output = multiply(ai->weights_arr[ai->layers - 2], hidden);
    activation_function(output);



    //save_vector_to_txt(output);
    free_vector(input);
    free_vector(hidden);
    free_vector(output);
    free_AI(ai);
}