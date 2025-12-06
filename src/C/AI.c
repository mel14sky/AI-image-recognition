#include <stdio.h>
#include <stdbool.h>
#include "matrix_logic.h"

//learning rate not saving to txt
//read from txt fucking up





int main(void) {

    struct AI* ai = create_AI_from_txt("output.txt");

    print_AI(ai);


    //randomize_AI_weights(ai);


    printf("w randomised\n");

    /*
    for (int i = 0; i < 3; i++) {
        train(ai, "training_data.txt", "answer_key.txt");
    }



    int player_input = 0;
    printf("now you test it, 69 is exit value\n");
    while (true) {
        scanf("%d", &player_input);
        if (player_input == 69) {break;}
        struct vector* input = make_vector(3);
        input->values[0] = (bool)(player_input / 10);
        input->values[1] = (bool)(player_input % 10);
        input->values[2] = 1;
        struct vector* output = feed_forward(input, ai);
        printf("output = ");
        print_v(output);
        int expected = (input->values[0] != input->values[1])*2 - 1;
        printf("expected = %d\n", expected);
        free_vector(output);
    }
    */

    save_AI_to_txt(ai, "output.txt");
    printf("saved\n");
    free_AI(ai);
    printf("freed\n");
}