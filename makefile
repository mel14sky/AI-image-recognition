trainable_AI: build src/C/AI.c src/C/matrix_logic.h 
	gcc src/C/AI.c -o build/AI_trainable

clean:
	rm -rf build

build:
	mkdir build
	
# ./build/AI_trainable training_location_file
# ./run python input
# ./build/AI input_JPG_location
# ./run python output