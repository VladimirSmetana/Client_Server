all: 
	g++ -Wall -o server server_dir/server.cpp socket.cpp -std=c++17 -lpthread
	g++ -Wall -o client client_dir/client.cpp -std=c++17 -lpthread