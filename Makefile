all: 
	g++ -Wall -o server server.cpp socket.cpp -std=c++20 -lpthread
	g++ -Wall -o client client.cpp -std=c++20 -lpthread