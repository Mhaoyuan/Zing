CC = g++ -g 
Zing:main.o util.o threadpool.o epoll.o http.o http_parse.o http_request.o priority_queue.o rio.o timer.o threadpool.o 
	$(CC) *.o -o Zing -lpthread

main.o:main.cpp
	$(CC) -c main.cpp
util.o:util.cpp
	$(CC) -c util.cpp
threadpool.o:threadpool.cpp
	$(CC) -c threadpool.cpp
epoll.o:epoll.cpp
	$(CC) -c epoll.cpp
http.o:http.cpp
	$(CC) -c http.cpp
http_parse.o:http_parse.cpp
	$(CC) -c http_parse.cpp
http_request.o:http_request.cpp
	$(CC) -c http_request.cpp
priority_queue.o:priority_queue.cpp
	$(CC) -c priority_queue.cpp
rio.o:rio.cpp
	$(CC) -c rio.cpp
timer.o:timer.cpp
	$(CC) -c timer.cpp
threadpool.o:threadpool.cpp
	$(CC) -c threadpool.cpp


clean:
	re -rf *.o Zing
