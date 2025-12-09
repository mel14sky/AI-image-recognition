AI: build src/C/AI.c src/C/matrix_logic.h 
	gcc src/C/AI.c -o build/AI

clean:
	rm -rf build

build:
	mkdir build
	
