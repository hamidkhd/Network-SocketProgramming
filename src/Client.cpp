#include "Client.hpp"

Client::Client() {
	try {			
		connect_to_server();		
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

Client::~Client() {}

void Client::connect_to_server() {
    //int addr_len = sizeof(this->serv_addr); 

    this->serv_addr.sin_family = AF_INET; 
	this->serv_addr.sin_port = htons(PORT);

    if ((this->sock_fd = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0) {
        throw SocketCreationFailed();
	}
	
	if (inet_pton(AF_INET, IP_ADDRESS, &this->serv_addr.sin_addr) <= 0)
        throw AddressFailed();

	if (connect(sock_fd, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr)) < 0)
        throw ConnectionFailed();
}

void Client::send_data_to_server(std::string data)
{
	send(sock_fd, data.data(), data.size(), 0);
}


int main(int argc, char const *argv[]) {
	Client *client = new Client();

	client->send_data_to_server("Salam ali.");
}