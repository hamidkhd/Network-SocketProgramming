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

	if ((this->new_command_socket = accept(this->command_socket, (struct sockaddr *)&this->command_addr, (socklen_t*)&command_addr_len)) < 0) 
		throw AcceptFailed();
}

std::string Server::recive_data_from_client()
{
	//int command_addr_len = sizeof(this->command_addr); 

	//if ((this->new_command_socket = accept(this->command_socket, (struct sockaddr *)&this->command_addr, (socklen_t*)&command_addr_len)) < 0) 
	//	throw AcceptFailed();

	char buffer[1024] = {0};

	// *** recive file with error ***

	//if (recv(this->new_command_socket, buffer, 1024, 0) < 0)
	//	throw ReciveDataFailed();

	recv(this->new_command_socket, buffer, 1024, 0);

	//close(this->new_command_socket);

	return buffer;
}

void Server::send_response_to_client(std::string data) {
	//if (send(this->new_command_socket, data.data(), data.size(), 0) < 0)
	//	throw SendDataFailed();
	send(this->new_command_socket, data.data(), data.size(), 0);
}

int main(int argc, char const *argv[]) {
	Server *server = new Server();

	while (true) {
		try {			
			std::string command = server->recive_data_from_client();
			std::string response = server->command_handler->run_command_handler(command);
			server->send_response_to_client(response);
		}
		catch (std::exception &ex) {
			std::cout << ex.what() << std::endl;
		}
	}

	return 0;	
}