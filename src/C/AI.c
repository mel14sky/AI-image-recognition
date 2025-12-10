#include <stdio.h>
#include <stdbool.h>
#include "matrix_logic.h"

//backtracking entering transpose step and stoping for some reason





int main(void) {

    //struct AI* ai = create_AI_from_txt("output.txt");
    int layers_arr[3] = {2, 5, 1};
    struct AI* ai = create_AI(layers_arr, 3, 0.05);
    randomize_AI_weights(ai);

    print_AI(ai);


    for (int i = 0; i < 3; i++) {
        train(ai, "training_data.txt", "answer_key.txt");
    }



   


    save_AI_to_txt(ai, "output.txt");
    free_AI(ai);
    printf("freed\n");

}