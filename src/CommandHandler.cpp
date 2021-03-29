#include "CommandHandler.hpp"


CommandHandler::CommandHandler() {
	this->data_base = new DataBase();
}

CommandHandler::~CommandHandler() {}

std::string CommandHandler::run_command_handler(std::string input) {
	try {
		separate_input_to_words(input);
		return handle_command();
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

std::string CommandHandler::handle_command() {
	if (input_words[0] == "user")
	{
		if (this->confirm_username(input_words[1]))
			return SUCCESS_USERNAME_FINDING;
		else 
			throw UserNotFound();
	}
	return ""; // for warning
}

bool CommandHandler::confirm_username(std::string username) {
	std::vector <User> users = this->data_base->get_users();

	for (long unsigned int i = 0; i < users.size(); i++) {
		if (username == users[i].get_username())
			return true;
	}
	return false;
}