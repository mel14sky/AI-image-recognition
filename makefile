run: build build/output_AI.txt build/AI src/Python/PokemonGUI.py src/Python/imadge2vec.py src/Python/BarChart.py
	python src/Python/PokemonGUI.py
	python src/Python/img2vec.py
	./build/AI
	python src/Python/BarChart.py

clean:
	rm -rf build

build:
	mkdir build
	
build/output_AI.txt: build src/C/AI.c src/C/matrix_logic.h 
	gcc src/C/AI_trainable.c -o build/AI_trainable
	./build/AI_trainable

build/AI: build scr/C/AI.c
	gcc src/C/AI.c -o build/AI

