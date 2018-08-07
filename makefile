CC = g++ -g 
Zing:main.o util.o threadpool.o
	$(CC) *.o -o Zing -lpthread

main.o:main.cpp
	$(CC) -c main.cpp
util.o:util.cpp
	$(CC) -c util.cpp
threadpool.o:threadpool.cpp
	$(CC) -c threadpool.cpp

clean:
	re -rf *.o Zing
