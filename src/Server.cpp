#include "Server.hpp"

Server::Server() {
	data_base = new DataBase();
	logger = new Logger();
	command_handler = new CommandHandler(data_base, logger);

	try {	
		initial_socket();
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

Server::~Server() {
	close(command_socket);
}

void Server::initial_socket() {

	int opt = 1;
	int command_addr_len = sizeof(command_addr); 
	command_addr.sin_family = AF_INET; 
	command_addr.sin_addr.s_addr = INADDR_ANY; 
	command_addr.sin_port = htons(data_base->get_command_port()); 

	if ((command_socket = socket(AF_INET , SOCK_STREAM , IP_PROTOCOL)) == 0) 
		throw SocketCreationFailed();

    if (setsockopt(command_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) 
		throw SetSockOptFailed(); 

	if (bind(command_socket, (struct sockaddr *)&command_addr, command_addr_len) < 0) 
		throw BindFailed(); 

    if (listen(command_socket, 3) < 0) 
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
					if ((new_socket =  accept(command_socket, (struct sockaddr *)&command_addr, (socklen_t*)&command_addr_len)) < 0) {
						throw AcceptFailed();
					}
                    FD_SET(new_socket, &master_set);
                    if (new_socket > max_sd)
                        max_sd = new_socket;
                }
                else { // new message from a client
					char buffer[1024] = {0};
					int temp;
					if ((temp = recv(i , buffer, 1024, 0)) < 0) {
						throw ReciveDataFailed();
					}
					else if (temp == 0) { // client disconnected
						close(i);
						User* user = data_base->get_user(i);
						data_base->remove_user_fd(i);
						data_base->remove_command_fd(i);
						if (user != nullptr && user->is_loggedin())
							user->logout();
						FD_CLR(i, &master_set);
					} else {
                    	std::string resp = command_handler->run_command_handler(buffer, i);
						if (send(i, resp.data(), resp.size(), 0) < 0)
							throw SendDataFailed();
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