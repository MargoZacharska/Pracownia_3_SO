all : program

program: main.o ConcurrentResource_Queue.o ConcurrentResource.o
	g++ -std=c++11  -pthread -Wall -Werror -o program main.o ConcurrentResource_Queue.o ConcurrentResource.o

main.o: main.cpp  
	g++ -std=c++11 -pthread -Wall -Wextra -Werror main.cpp -c

ConcurrentResource_Queue.o: ConcurrentResource_Queue.cpp ConcurrentResource_Queue.h
	g++ -std=c++11 -pthread -Wall -Wextra -Werror ConcurrentResource_Queue.cpp -c
	
ConcurrentResource.o: ConcurrentResource.cpp ConcurrentResource.h
	g++ -std=c++11 -pthread -Wall -Wextra -Werror ConcurrentResource.cpp -c
	
run :
	./program 10 10 10 10

