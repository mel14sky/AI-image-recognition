#include <stdio.h>
#include "matrix_logic.h"



#define number_of_layers 3


int main(void) {

    //struct AI* ai = create_AI_from_txt("output.txt");
    int layers_arr[number_of_layers] = {2, 5, 1};
    struct AI* ai = create_AI(layers_arr, number_of_layers, 0.05);
    randomize_AI_weights(ai);

    //print_AI(ai);
    printf("matrixes = %d\nvectors = %d\n", matrixes, vectors);

    for (int i = 0; i < 5; i++) {
        train(ai, "XOR.txt");
        printf("matrixes = %d\nvectors = %d\n", matrixes, vectors);
        printf("\n\n");
    }



    save_AI_to_txt(ai, "output.txt");
    free_AI(ai);
    printf("freed\n");
    printf("matrixes = %d\nvectors = %d\n", matrixes, vectors);
}