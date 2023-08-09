all: compile link open clean

compile:
	g++ -I headers -I src\include -c main.cpp -o main.o 

link:
	g++ -L src\lib .\main.o -o Game.exe -lmingw32 -lsfml-network -lsfml-graphics  -lsfml-audio -lsfml-window -lsfml-system -lsfml-main

open:
	.\Game.exe

clean:
	rm Game.exe
	rm main.o