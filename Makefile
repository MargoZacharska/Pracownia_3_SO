all : program

program : main.cpp
	g++ -std=c++11 -pthread -Wall -Wextra -Werror main.cpp -o program
	
run :
	./program 10 10 5 10

