#include <stdio.h>
#include "matrix_logic.h"

//1 = false
//2 = true
//3 = true
//4 = false

#define number_of_layers 6


int main(void) {

    warnings = 0;


    //struct AI* ai = create_AI_from_txt("output.txt");
    const int layers_arr[number_of_layers] = {2, 10, 5, 6, 3, 1};
    struct AI* ai = create_AI(layers_arr, number_of_layers, 0.05);
    randomize_AI_weights(ai);

    print_AI(ai);
    printf("matrix = %d\nvectors = %d\n", matrix_count, vectors);

    for (int i = 0; i < 40; i++) {
        train(ai, "XOR.txt");
        //print_AI(ai);
        printf("matrix = %d\nvectors = %d\n", matrix_count, vectors);
    }

    int num = 0;
    while (num != 69) {
        printf("number please\n");
        scanf("%d", &num);
        struct vector* input = make_vector(3);
        input->values[0] = num/10;
        input->values[1] = num%10;
        input->values[2] = 1;
        struct feed_forward_return* test = feed_forward(input, ai);
        print_v(test->output);

        free_feed_forward_return(test, ai);
        free_vector(input);
    }



    save_AI_to_txt(ai, "output.txt");
    free_AI(ai);
    printf("freed\n");
    printf("matrix = %d\nvectors = %d\n", matrix_count, vectors);

}