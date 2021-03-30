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
    this->command_addr.sin_family = AF_INET; 
	this->command_addr.sin_port = htons(COMMAND_PORT);

    if ((this->command_socket = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0) {
        throw SocketCreationFailed();
	}
	
	if (inet_pton(AF_INET, IP_ADDRESS, &this->command_addr.sin_addr) <= 0)
        throw AddressFailed();

	if (connect(command_socket, (struct sockaddr *)&this->command_addr, sizeof(this->command_addr)) < 0)
        throw ConnectionFailed();

	data_addr.sin_family = AF_INET; 
    data_addr.sin_port = htons(DATA_PORT); 
    data_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	data_socket = socket(AF_INET, SOCK_STREAM, 0);
}

void Client::connect_data_sock() {

}

void Client::send_data_to_server(std::string data)
{
	if (send(this->command_socket, data.data(), data.size(), 0) < 0)
		throw SendDataFailed();
}

std::string Client::receive_response_from_client() {
	char buffer[1024] = {0};
	recv(this->command_socket, buffer, 1024, 0);
	return buffer;
}

void Client::handle_response(std::string res) {
	if (res == "connect") {
		connect(data_socket, (struct sockaddr *)&data_addr, sizeof(data_addr));
		handle_response(receive_response_from_client());
		return;
	} else {
		std::cout << res << std::endl;
	}
}


void Client::run() {
	std::string command, response;
	while (true) {
		try {			
			getline(std::cin, command);
			send_data_to_server(command);	
			response = receive_response_from_client();
			handle_response(response);
		}
		catch (std::exception &ex) {
			std::cout << ex.what() << std::endl;
		}
    }
}

int main(int argc, char const *argv[]) {
	Client *client = new Client();
	client->run();
	return 0;
}