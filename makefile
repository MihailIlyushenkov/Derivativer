CC = g++

all: compile

compile: main.o tree.o Rec.o
	$(CC) Objects\main.o Objects\tree.o Objects\Rec.o -o main.exe

main.o:
	$(CC) -c Sources\main.cpp -o Objects\main.o

tree.o:
	$(CC) -c Sources\tree.cpp -o Objects\tree.o

Rec.o:
	$(CC) -c Sources\Rec.cpp -o Objects\Rec.o

clean:
	del Objects\*.o *.exe
