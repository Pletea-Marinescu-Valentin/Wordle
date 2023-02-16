build:wordle
wordle:wordle.c
	gcc -o wordle wordle.c -lncurses
clean:
	rm wordle
