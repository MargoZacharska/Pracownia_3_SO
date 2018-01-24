all : program

program: main.o ConcurrentResource_Queue.o ConcurrentResource.o

main.o: main.cpp  
	g++ -std=c++11 -pthread -Wall -Wextra -Werror main.cpp -o program

ConcurrentResource_Queue.o: ConcurrentResource_Queue.cpp ConcurrentResource_Queue.h
	g++ -std=c++11 -Wall -Wextra -Werror ConcurrentResource_Queue.cpp -o ConcurrentResource_Queue.o
	
ConcurrentResource.o: ConcurrentResource.cpp ConcurrentResource.h
	g++ -std=c++11 -Wall -Wextra -Werror ConcurrentResource.cpp -o ConcurrentResource.o
	
run :
	./program 10 10 10 10



