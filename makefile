CC = g++

all: compile

compile: main.o tree.o
	$(CC) Objects\main.o Objects\tree.o -o main.exe

main.o:
	$(CC) -c Sources\main.cpp -o Objects\main.o

tree.o:
	$(CC) -c Sources\tree.cpp -o Objects\tree.o

clean:
	del Objects\*.o *.exe
