#include "CommandHandler.hpp"


CommandHandler::CommandHandler() {
	this->data_base = new DataBase();
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
	return ""; //for warning
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

	//for (long unsigned int i = 0; i < input_words.size(); i++)
	//	std::cout << input_words[i] << std::endl;
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
		return LOGIN_SUCCESS;
		
	} else if (input_words[0] == "pwd") {
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
		return LOGOUT_SUCCESS;
	}

	throw WritingError();
	return ""; // for warning
}