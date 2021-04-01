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
	close(data_socket);
	close(command_socket);
}

void Client::connect_to_server() {
    command_addr.sin_family = AF_INET; 
	command_addr.sin_port = htons(COMMAND_PORT);

    if ((command_socket = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0) {
        throw SocketCreationFailed();
	}
	
	if (inet_pton(AF_INET, IP_ADDRESS, &command_addr.sin_addr) <= 0)
        throw AddressFailed();

	if (connect(command_socket, (struct sockaddr *)&command_addr, sizeof(command_addr)) < 0)
        throw ConnectionFailed();

	data_addr.sin_family = AF_INET; 
    data_addr.sin_port = htons(DATA_PORT); 
    data_addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	if ((data_socket = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0) {
        throw SocketCreationFailed();
	}
}

void Client::send_data_to_server(std::string data)
{
	if (send(command_socket, data.data(), data.size(), 0) < 0)
		throw SendDataFailed();
}

std::string Client::receive_response_from_server(int sock) {
	char buffer[1024] = {0};
	if (recv(sock, buffer, 1024, 0) < 0)
		throw ReciveDataFailed();
	return buffer;
}

void Client::receive_file_from_server(std::string res) {
	char buffer[1024] = {0};
	mkdir("downloads", 0777);
	int len = res.find("$") - res.find("#") - 1;
	off_t file_len = std::stol(res.substr(res.find('#') +1, len));
	std::string file_path = "downloads/" + res.substr(3,  res.find('#')-3);
	int file_fd = open(file_path.c_str(), O_RDWR | O_CREAT, 0777);
	int total_read = 0;
	int last_read = 0;
	while (total_read < file_len) {
		if ((last_read = recv(data_socket, buffer, 1024, 0)) < 0) {
			throw ReciveDataFailed();
		}
		write(file_fd, buffer, last_read);
		memset(buffer, 0, 1024);
		total_read += last_read;
	}
    close(file_fd);
}

void Client::handle_response(std::string res) {
	if (res == "connect") {
		if (connect(data_socket, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0)
        	throw ConnectionFailed();
		handle_response(receive_response_from_server(command_socket));
		return;
	} else if (res.length() > 1 && res.substr(0,2) == "ls") {
		std::string data_res = receive_response_from_server(data_socket);
		std::cout << data_res << std::endl;
		if (res.length() == 2)
			handle_response(receive_response_from_server(command_socket));
		else 
			std::cout << res.substr(2) << std::endl;	
		return;
	} else if (res.length() > 1 && res.substr(0,2) == "dl") {
		receive_file_from_server(res);
		if (res.length() == res.find('$')+1)
			handle_response(receive_response_from_server(command_socket));
		else 
			std::cout << res.substr(res.find('$')+1) << std::endl;	
		return;
	} else if (res.substr(0,3) == "221") {
		close(data_socket);
		if ((data_socket = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL)) < 0) {
        	throw SocketCreationFailed();
		}
	} else if (res.length() > 1 && res.substr(0,2) == "hp") {
		off_t msg_len = std::stol(res.substr(3, res.find('$') - 2));
		int total_read = res.size() > 1024 ? 1024 - res.find('$') - 1 : res.size() - res.find('$') - 1;
		int last_read = 0;
		if (total_read > 0)
			std::cout << res.substr(res.find('$') + 1, total_read);
		
		char buffer[1024] = {0};
		while (total_read < msg_len) {
			if ((last_read = recv(command_socket, buffer, 1024, 0)) < 0) {
			throw ReciveDataFailed();
			}
			printf("%.*s", last_read, buffer);
			memset(buffer, 0, 1024);
			total_read += last_read;
		}
		return;
	}
	
	std::cout << res << std::endl;
}


void Client::run() {
	std::string command, response;
	while (true) {
		try {			
			getline(std::cin, command);
			send_data_to_server(command);	
			response = receive_response_from_server(command_socket);
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