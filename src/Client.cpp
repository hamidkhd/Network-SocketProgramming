#include "Client.hpp"

Client::Client() {
	try {			
		connect_to_server();		
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

Client::~Client() {
	close(this->command_socket);
}

void Client::connect_to_server() {
    //int addr_len = sizeof(this->command_addr); 

    this->command_addr.sin_family = AF_INET; 
	this->command_addr.sin_port = htons(COMMAND_PORT);

    if ((this->command_socket = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0) {
        throw SocketCreationFailed();
	}
	
	if (inet_pton(AF_INET, IP_ADDRESS, &this->command_addr.sin_addr) <= 0)
        throw AddressFailed();

	if (connect(command_socket, (struct sockaddr *)&this->command_addr, sizeof(this->command_addr)) < 0)
        throw ConnectionFailed();
}

void Client::send_data_to_server(std::string data)
{
	if (send(this->command_socket, data.data(), data.size(), 0) < 0)
		throw SendDataFailed();
}

std::string Client::recive_response_from_client() {
	char buffer[1024] = {0};
	recv(this->command_socket, buffer, 1024, 0);
	return buffer;
}

std::string Client::get_input_from_user() {
    std::getline(std::cin, this->input_command);
	return input_command;
}


int main(int argc, char const *argv[]) {
	Client *client = new Client();

	while (true) {
		try {			
			std::string command = client->get_input_from_user();	
			client->send_data_to_server(command);	
			std::cout << client->recive_response_from_client() << std::endl;
		}
		catch (std::exception &ex) {
			std::cout << ex.what() << std::endl;
		}
    }

	return 0;
}