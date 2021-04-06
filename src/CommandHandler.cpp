#include "CommandHandler.hpp"


CommandHandler::CommandHandler(DataBase* _data_base, Logger* _logger):
	data_base(_data_base), logger(_logger) {
	
	int opt = 1;

    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = INADDR_ANY;
    data_addr.sin_port = htons(data_base->get_data_port());

	if ((data_socket = socket(AF_INET , SOCK_STREAM , IP_PROTOCOL)) == 0) {
		throw SocketCreationFailed();
	}

    if (setsockopt(data_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0) 
		throw SetSockOptFailed(); 
       
	if (bind(data_socket, (struct sockaddr *)&data_addr, sizeof(data_addr)) < 0) 
		throw BindFailed();

	if (listen(data_socket, 4) < 0) 
		throw ListenFailed();
}

CommandHandler::~CommandHandler() {
	close(data_socket);
}

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
	input_words.clear();
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
	int addr_len = sizeof(client_addr);
	memset(&client_addr, 0, sizeof(client_addr));
	int new_socket;
	if ((new_socket = accept(data_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len)) < 0) {
		throw AcceptFailed();
	}
	return new_socket;
}

void CommandHandler::mkd(int client_fd, User* user) {
	if (user == nullptr || !user->is_loggedin())
		throw UserNotLoggin();
	if (input_words.size() < 2)
		throw WritingError();
	std::string path = user->get_cwd() + "/" + input_words[1];
	mkdir(path.c_str(), 0777);
}

void CommandHandler::dele(User* user) {
	if (user == nullptr || !user->is_loggedin())
		throw UserNotLoggin();
	if (input_words.size() < 3)
		throw WritingError();
	if (input_words[2].find('/') != std::string::npos)
		throw WritingError();
	
	struct stat sb;
	std::string path = user->get_cwd() + "/" + input_words[2];
	if (input_words[1] == "-f") {
		if (data_base->is_restricted(input_words[2]) && !user->is_admin())
		throw IllegalAccess();
		
		if (stat(path.c_str(), &sb) == 0 && S_ISREG(sb.st_mode))
			remove(path.c_str());
		else
			throw WritingError();
		logger->save_log("User with username: '" + user->get_username() + "' delete file with name: '" + input_words[2] +  "' successfully.");
	} else if (input_words[1] == "-d") {
		if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
			system(("rm -rf " + path).c_str());
		else 
			throw WritingError();
		logger->save_log("User with username: '" + user->get_username() + "' delete directory with path: '" + input_words[2] +  "' successfully.");
	} else {
		throw WritingError();
	}
}

void CommandHandler::ls(int client_fd, User* user) {
	if (user == nullptr || !user->is_loggedin())
		throw UserNotLoggin();
	
	int data_fd = data_base->get_command_fd(client_fd);
	char tmp[100] = {0};
	strcpy(tmp, "ls");
	if (send(client_fd, tmp, strlen(tmp), 0) < 0)
		throw SendDataFailed();
	std::vector<std::string> dir_list;
	std::string result;
    DIR *d;
    struct dirent *dir;
    d = opendir(user->get_cwd().c_str());
    if (d) {
        while ((dir = readdir(d)) != NULL)
            dir_list.push_back(dir->d_name);
        closedir(d);
    }
    sort(dir_list.begin(), dir_list.end());

    for(std::string dir_name: dir_list) {
    	result.append(dir_name + "\n");
	}

	if (fork() == 0) {
		if (send(data_fd, result.data(), result.size(), 0) < 0)
			throw SendDataFailed();
		exit(0);
	}
}

void CommandHandler::cwd(User* user) {
	if (user == nullptr || !user->is_loggedin())
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
}

void CommandHandler::rename_command(User* user) {
	if (user == nullptr || !user->is_loggedin())
		throw UserNotLoggin();
	if (input_words.size() < 3)
		throw WritingError();
	if (data_base->is_restricted(input_words[1]) && !user->is_admin())
		throw IllegalAccess();
	if (input_words[1].find('/') != std::string::npos ||
		input_words[2].find('/') != std::string::npos)
		throw WritingError();

	std::string file_name = user->get_cwd() + "/" + input_words[1];
	std::string new_name = user->get_cwd() + "/" + input_words[2];
	if (std::rename(file_name.c_str(), new_name.c_str()) < 0)
		throw WritingError();
	logger->save_log("User with username: '" + user->get_username() + "' rename file with name: '" + input_words[1] +  "' to new name: '" + input_words[2] + "' successfully.");
}

void CommandHandler::retr(int client_fd, User* user) {
	if (user == nullptr || !user->is_loggedin())
		throw UserNotLoggin();
	if (input_words.size() < 2)
		throw WritingError();
	if (data_base->is_restricted(input_words[1]) && !user->is_admin())
		throw IllegalAccess();
	if (input_words[1].find('/') != std::string::npos)
		throw WritingError();
	
	struct stat sb;
	std::string path = user->get_cwd() + "/" + input_words[1];
	if (stat(path.c_str(), &sb) != 0 || !S_ISREG(sb.st_mode))
		throw WritingError();
	if (!user->can_download(sb.st_size))
		throw FullCapacity();
	user->subtract_size(sb.st_size);
	
	int data_fd = data_base->get_command_fd(client_fd);
	struct stat stat_buf; 
	int file_fd = open(path.c_str() , O_RDONLY);
    fstat (file_fd, &stat_buf);
	char tmp[100] = {0};
	strcpy(tmp, "dl ");
	strcat(tmp, input_words[1].c_str());
	strcat(tmp, "#");
	strcat(tmp, std::to_string(stat_buf.st_size).c_str());
	strcat(tmp, "$");

	if (send(client_fd, tmp, strlen(tmp), 0) < 0)
		throw SendDataFailed();

	if (fork() == 0) {
    	sendfile(data_fd, file_fd, NULL, stat_buf.st_size);
    	close(file_fd);
		close(data_fd);
		exit(0);
	}
	logger->save_log("User with username: '" + user->get_username() + "' download file with name: '" + input_words[1] +  "' successfully.");
	close(file_fd);
}

void CommandHandler::user_command(int client_fd, User* user) {
	if (input_words.size() == 1)
		throw WritingError();
	if (user != nullptr && user->is_loggedin())
    	throw BadSequence();
	user = data_base->get_user(input_words[1]);
	if (user->is_loggedin())
		throw BadSequence();
	data_base->set_user_fd(client_fd, user);
}

void CommandHandler::pass_command(int client_fd, User* user) {
	if (input_words.size() == 1)
		throw WritingError();
	if (user == nullptr || user->is_loggedin())
		throw BadSequence();
	user->login(input_words[1]);
	int sock = create_data_connection(client_fd);
	data_base->set_command_fd(client_fd, sock);
}

std::string CommandHandler::help(int client_fd) {
	std::string message;

	message += "214\n";
	message += "1) USER [name], Its argument is used to specify the user's string. It is used for user authentication.\n";
	message += "2) Pass [password], Its argument is used to specify the user's password. It is used for user login.\n";
	message += "3) PWD, It is used for get the path of current working directory.\n";
	message += "4) MKD [directory path], Its argument is used to specify the new directory's path. It is used for creat new directory in specified path\n";
	message += "5) DELE -f [file name], Its argument is used to specify the file's name. It is used for delete file with specified name\n";
	message += "6) DELE -d [directory path], Its argument is used to specify the directory's path. It is used for delete directory with specified path\n";
	message += "7) LS, It is used for view files in the current working directory.\n";
	message += "8) CWD [path], Its argument is used to specify the new directory's path. It is used for change the current working directory to specified path.";
	message += "If the argument equals to '..' it goes to previous directory and if no argument is entered, it goes to the first directory.\n";
	message += "9) RENAME [from] [to], First argument is used to specify the file's name and second argument is used to specify new name for that file.";
	message += "It is used for reaname file, if available.\n";
	message += "10) RETR [name], Its argument is used to specify the file's name. It is used for download file with the given name, if available.\n";
	message += "11) HELP, It is used to display commands on the server along with instructions for using them.\n";
	message += "12) QUIT, It is used for logout and remove current user from the system.\n";

	char tmp[100] = {0};
	strcpy(tmp, "hp ");
	strcat(tmp, std::to_string(message.size()).c_str());
	strcat(tmp, "$");

	if (send(client_fd, tmp, strlen(tmp), 0) < 0)
		throw SendDataFailed();

	return message;
}

void CommandHandler::quit(int client_fd, User* user) {
	if (user == nullptr || !user->is_loggedin())
		throw BadSequence();
	user->logout();
	data_base->remove_user_fd(client_fd);
	data_base->remove_command_fd(client_fd);
}

std::string CommandHandler::handle_command(int client_fd) {
	User* user = data_base->get_user(client_fd);

	if (input_words[0] == "user") {
		user_command(client_fd, user);
		return SUCCESS_USERNAME_FINDING;
	} 
	else if (input_words[0] == "pass") {
		pass_command(client_fd, user);
		logger->save_log("User with username: '" + user->get_username() + "' entered successfully.");
		return LOGIN_SUCCESS;
	} 
	
	else if (input_words[0] == "pwd") {
		if (user == nullptr || !user->is_loggedin())
			throw UserNotLoggin();
		return "257: " + user->get_cwd();
	} 
	
	else if (input_words[0] == "mkd") {
		mkd(client_fd, user);
		std::string path = user->get_cwd() + "/" + input_words[1];
		logger->save_log("User with username: '" + user->get_username() + "' create directory or file with path: '" + input_words[1] +  "' successfully.");
		return "257: " + path + " created.";
	} 
	
	else if (input_words[0] == "dele") {
		dele(user);
		std::string path = user->get_cwd() + "/" + input_words[2];
		return "250: " + path + " deleted";
	} 
	
	else if (input_words[0] == "ls") {
		ls(client_fd, user);
		return LIST_TRANSFER_SUCCESS;
	} 
	
	else if (input_words[0] == "cwd") { // TODO: remove .. from string
		cwd(user);
		return CHANGE_SUCCESS;
	} 
	
	else if (input_words[0] == "rename") {
		rename_command(user);
		return CHANGE_SUCCESS;
	} 
	
	else if (input_words[0] == "retr") {
		retr(client_fd, user);
		return DOWNLOAD_SUCCESS;	
	} 
	
	else if (input_words[0] == "help") { 
		return help(client_fd);
	} 
	
	else if (input_words[0] == "quit") {
		quit(client_fd, user);
		logger->save_log("User with username: '" + user->get_username() + "' exit successfully.");
		return LOGOUT_SUCCESS;
	}

	throw WritingError();
}