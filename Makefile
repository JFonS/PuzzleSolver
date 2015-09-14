all: main 
	./puzzlesolver

main: main.cpp
	g++ -o puzzlesolver main.cpp -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm *.o puzzlesolver

