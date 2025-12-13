#include <stdio.h>
#include <stdbool.h>
#include "matrix_logic.h"



#define number_of_layers 3


int main(void) {

    //struct AI* ai = create_AI_from_txt("output.txt");
    int layers_arr[number_of_layers] = {2, 5, 1};
    struct AI* ai = create_AI(layers_arr, number_of_layers, 0.01);
    randomize_AI_weights(ai);

    print_AI(ai);


    for (int i = 0; i < 50; i++) {
        train(ai, "training_data1.txt", "answer_key1.txt");
        train(ai, "training_data2.txt", "answer_key2.txt");
        train(ai, "training_data3.txt", "answer_key3.txt");
        train(ai, "training_data4.txt", "answer_key4.txt");
        double error = 0;
        struct vector* input = make_vector(3);
        input->values[0] = 1;
        input->values[1] = 1;
        input->values[2] = 1;
        struct vector* output = feed_forward(input, ai);
        error += fabs(output->values[0] - (input->values[0] != input->values[1]));
        free_vector(output);
        input->values[0] = 1;
        input->values[1] = 0;
        output = feed_forward(input, ai);
        error += fabs(output->values[0] - (input->values[0] != input->values[1]));
        free_vector(output);
        input->values[0] = 0;
        input->values[1] = 1;
        output = feed_forward(input, ai);
        error += fabs(output->values[0] - (input->values[0] != input->values[1]));
        free_vector(output);
        input->values[0] = 0;
        input->values[1] = 0;
        output = feed_forward(input, ai);
        error += fabs(output->values[0] - (input->values[0] != input->values[1]));
        free_vector(output);
        free_vector(input);
        error = error / 4;
        printf("error = %f\n", error);
    }

    printf("\n");
    struct vector* input = make_vector(3);
    input->values[0] = 1;
    input->values[1] = 1;
    input->values[2] = 1;
    struct vector* output = feed_forward(input, ai);
    printf("output 11 = %f\n", output->values[0]);
    free_vector(output);
    input->values[0] = 1;
    input->values[1] = 0;
    output = feed_forward(input, ai);
    printf("output 10 = %f\n", output->values[0]);
    free_vector(output);
    input->values[0] = 0;
    input->values[1] = 1;
    output = feed_forward(input, ai);
    printf("output 01 = %f\n", output->values[0]);
    free_vector(output);
    input->values[0] = 0;
    input->values[1] = 0;
    output = feed_forward(input, ai);
    printf("output 00 = %f\n", output->values[0]);
    free_vector(output);
    free_vector(input);



    save_AI_to_txt(ai, "output.txt");
    free_AI(ai);
    printf("freed\n");

}