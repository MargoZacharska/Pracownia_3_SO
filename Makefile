all : program

program : main.cpp
	g++ -std=c++11 -pthread -Wall -Wextra -Werror main.cpp -o program
	
run :
	./program 5 10 5 10

