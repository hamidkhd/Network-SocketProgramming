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

std::string Client::receive_response(int sock) {
	char buffer[1024] = {0};
	recv(sock, buffer, 1024, 0);
	return buffer;
}

void Client::receive_file(std::string res) {
	char buffer[1024] = {0};
	mkdir("downloads", 0777);
	int len = res.find("$") - res.find("#") - 1;
	off_t file_len = std::stol(res.substr(res.find('#') +1, len));
	std::string file_path = "downloads/" + res.substr(3,  res.find('#')-3);
	int file_fd = open(file_path.c_str(), O_RDWR | O_CREAT, 0777);
	int total_read = 0;
	int last_read = 0;
	while (total_read < file_len) {
		last_read = recv(data_socket, buffer, 1024, 0);
		write(file_fd, buffer, last_read);
		memset(buffer, 0, 1024);
		total_read += last_read;
	}
    close(file_fd);
}

void Client::handle_response(std::string res) {
	if (res == "connect") {
		connect(data_socket, (struct sockaddr *)&data_addr, sizeof(data_addr));
		handle_response(receive_response(command_socket));
		return;
	} else if (res.length() > 1 && res.substr(0,2) == "ls") {
		std::string data_res = receive_response(data_socket);
		std::cout << data_res << std::endl;
		if (res.length() == 2)
			handle_response(receive_response(command_socket));
		else 
			std::cout << res.substr(2) << std::endl;	
		return;
	} else if (res.length() > 1 && res.substr(0,2) == "dl") {
		receive_file(res);
		if (res.length() == res.find('$')+1)
			handle_response(receive_response(command_socket));
		else 
			std::cout << res.substr(res.find('$')+1) << std::endl;	
		return;
	} else if (res.substr(0,3) == "221") {
		close(data_socket);
		data_socket = socket(AF_INET, SOCK_STREAM, 0);
	}
	
	
	std::cout << res << std::endl;
}


void Client::run() {
	std::string command, response;
	while (true) {
		try {			
			getline(std::cin, command);
			send_data_to_server(command);	
			response = receive_response(command_socket);
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