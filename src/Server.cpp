#include "Server.hpp"

#define MAX_PLAYER 50

Server::Server() {
	try {	
		initial_socket();
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

Server::~Server() {}

void Server::initial_socket() {

	int opt = 1;
	int addr_len = sizeof(this->serv_addr); 
	this->serv_addr.sin_family = AF_INET; 
	this->serv_addr.sin_addr.s_addr = INADDR_ANY; 
	this->serv_addr.sin_port = htons(PORT); 

	if ((this->server_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
		throw SocketCreationFailed();

    if (setsockopt(this->server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) 
		throw SetSockOptFailed(); 

	if (bind(this->server_socket, (struct sockaddr *)&serv_addr, addr_len) < 0) 
		throw BindFailed(); 

    if (listen(this->server_socket, 3) < 0) 
		throw ListenFailed();
}

void Server::get_data_from_client()
{
	int addr_len = sizeof(this->serv_addr); 

	if ((this->new_socket = accept(this->server_socket, (struct sockaddr *)&this->serv_addr, (socklen_t*)&addr_len))<0) 
		throw AcceptFailed();

	char buffer[1024] = {0};
	read(new_socket, buffer, 1024);
}

int main(int argc, char const *argv[]) {
	Server *server = new Server();
	
	server->get_data_from_client();
}