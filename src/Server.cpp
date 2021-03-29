#include "Server.hpp"

#define MAX_PLAYER 50

Server::Server() {
	this->command_handler = new CommandHandler();

	try {	
		initial_socket();
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

Server::~Server() {
	close(this->command_socket);
}

void Server::initial_socket() {

	int opt = 1;
	int command_addr_len = sizeof(this->command_addr); 
	this->command_addr.sin_family = AF_INET; 
	this->command_addr.sin_addr.s_addr = INADDR_ANY; 
	this->command_addr.sin_port = htons(COMMAND_PORT); 

	if ((this->command_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
		throw SocketCreationFailed();

    if (setsockopt(this->command_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) 
		throw SetSockOptFailed(); 

	if (bind(this->command_socket, (struct sockaddr *)&command_addr, command_addr_len) < 0) 
		throw BindFailed(); 

    if (listen(this->command_socket, 3) < 0) 
		throw ListenFailed();
}


void Server::run() {
	int command_addr_len = sizeof(command_addr); 
	fd_set master_set, working_set;
	int new_socket;

	FD_ZERO(&master_set);
	int max_sd = command_socket;
	FD_SET(command_socket, &master_set);

	while(true) {
		working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);

        for (int i = 0; i <= max_sd; i++) {
            if (FD_ISSET(i, &working_set)) {
                if (i == command_socket) { // new client connected
                    new_socket =  accept(this->command_socket, (struct sockaddr *)&this->command_addr, (socklen_t*)&command_addr_len);
                    FD_SET(new_socket, &master_set);
                    if (new_socket > max_sd)
                        max_sd = new_socket;
                }
                else { // new message from a client
					char buffer[1024] = {0};
                    if (recv(i , buffer, 1024, 0) == 0) {
						close(i);
						FD_CLR(i, &master_set);
					} else {
                    	std::string resp = command_handler->run_command_handler(buffer, i);
						send(i, resp.data(), resp.size(), 0);
					}
                }
            }
        }
	}
	
}

int main(int argc, char const *argv[]) {
	Server *server = new Server();

	server->run();	

	return 0;	
}