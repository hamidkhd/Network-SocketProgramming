#include "CommandHandler.hpp"


CommandHandler::CommandHandler() {
	this->data_base = new DataBase();
	int opt = 1;
	struct sockaddr_in server_addr;
	data_socket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(data_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // opt = fcntl(data_socket, F_GETFL);
    // opt = (opt | O_NONBLOCK);
    // fcntl(data_socket, F_SETFL, opt);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(DATA_PORT);
       
    bind(data_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));

    listen(data_socket, 4);
}

CommandHandler::~CommandHandler() {}

std::string CommandHandler::run_command_handler(std::string input, int client_fd) {
	try {
		separate_input_to_words(input);
		return handle_command(client_fd);
	}
	catch (std::exception &ex) {
		return ex.what();
	}
}

void CommandHandler::separate_input_to_words(std::string input) {
	this->input_words.clear();
	std::string temp_word = "";

    for (auto x : input) {
        if (x == ' ') {
            input_words.push_back(temp_word);
            temp_word = "";
        }
        else 
            temp_word += x;
    }
    input_words.push_back(temp_word);
}

int CommandHandler::create_data_connection(int fd) {
	char buff[100] = {0};
	strcpy(buff, "connect");
	send(fd, buff, strlen(buff), 0);
	struct sockaddr_in client_addr;
	int addrlen = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
    int new_socket = accept(data_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
	if (new_socket < 0) 
		throw ConnectionFailed();
	return new_socket;
}

std::string CommandHandler::handle_command(int client_fd) {
	User* user = data_base->get_user(client_fd);
	if (input_words[0] == "user") {
		if (input_words.size() == 1)
			throw WritingError();
		user = this->data_base->get_user(input_words[1]);
		this->data_base->set_user_fd(client_fd, user);
		return SUCCESS_USERNAME_FINDING;
	} else if (input_words[0] == "pass") {
		if (input_words.size() == 1)
			throw WritingError();
		if (user == nullptr)
			throw BadSequence();
		user->login(input_words[1]);
		int sock = create_data_connection(client_fd);
		data_base->set_command_fd(client_fd, sock);
		return LOGIN_SUCCESS;
		
	} else if (input_words[0] == "pwd") { // TODO: Fix allowing user who used "user" but hasn't entered "pass"
		if (user == nullptr)
			throw UserNotLoggin();
		return "257: " + user->get_cwd();
	} else if (input_words[0] == "mkd") {
		if (user == nullptr)
			throw UserNotLoggin();
		if (input_words.size() < 2)
			throw WritingError();
		std::string path = user->get_cwd() + "/" + input_words[1];
		mkdir(path.c_str(), 0777);
		return "257: " + path + " created.";
	} else if (input_words[0] == "dele") {
		if (user == nullptr)
			throw UserNotLoggin();
		if (input_words.size() < 3)
			throw WritingError();
		
		struct stat sb;
		std::string path = user->get_cwd() + "/" + input_words[2];
		if (input_words[1] == "-f") {
			if (stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
				remove(path.c_str());
			else
				throw WritingError();
		} else if (input_words[1] == "-d") {
			if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
				system(("rm -rf " + path).c_str());
			else 
				throw WritingError();
		} else {
			throw WritingError();
		}

		return "250: " + path + " deleted";

	} else if (input_words[0] == "ls") {

	} else if (input_words[0] == "cwd") {
		if (user == nullptr)
			throw UserNotLoggin();
		if (input_words.size() == 1) {
			user->set_cwd(getenv("PWD"));
		} else {
			struct stat sb;
			std::string path = user->get_cwd() + "/" + input_words[1];
			std::cout << path << std::endl;
			if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
				user->set_cwd(path);
			else
				throw WritingError();
		}
		return CHANGE_SUCCESS;
	} else if (input_words[0] == "rename") {
		if (user == nullptr)
			throw UserNotLoggin();
		if (input_words.size() < 3)
			throw WritingError();
		std::string file_name = user->get_cwd() + "/" + input_words[1];
		std::string new_name = user->get_cwd() + "/" + input_words[2];
		if (std::rename(file_name.c_str(), new_name.c_str()) < 0)
			throw WritingError();
		return CHANGE_SUCCESS;
	} else if (input_words[0] == "retr") {

	} else if (input_words[0] == "help") {

	} else if (input_words[0] == "quit") {
		if (user == nullptr)
			throw BadSequence();
		user->logout();
		data_base->remove_user_fd(client_fd);
		return LOGOUT_SUCCESS;
	}

	throw WritingError();
	return ""; // for warning
}