CC = gcc

all: main.c
	$(CC) -g -o main main.c myfilesystem.c bmsearch.c
	#./main
