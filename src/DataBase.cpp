#include "DataBase.hpp"

using json = nlohmann::json;

DataBase::DataBase() {
    readFile("config.json");
    parse_json();
}

DataBase::~DataBase() {}

void DataBase::readFile(std::string file_name) {
    this->json_file = std::ifstream(file_name);
}

void DataBase::parse_json() {

    nlohmann::json json_data;

    this->json_file >> json_data;

    std::vector <std::string> _accessible_files, empty;

    for (auto& admin_file : json_data["files"]) {
        _accessible_files.push_back(admin_file.get<std::string>());
    }

    for (auto& user_data : json_data["users"]) {
        std::string username = user_data["user"].get<std::string>();
        std::string password = user_data["password"].get<std::string>();
        std::string admin_status = user_data["admin"].get<std::string>();
        std::string size = user_data["size"].get<std::string>();

        if (admin_status == "true")
            this->users.push_back(new User(username, password, admin_status, stoi(size), _accessible_files));
        else 
            this->users.push_back(new User(username, password, admin_status, stoi(size), empty));
    }

}

std::vector <User*> DataBase::get_users() {
    return users;
}

User* DataBase::get_user(std::string username) {
    for (User* user: users) {
		if (username == user->get_username())
			return user;
    }
    throw UserNotFound();
}

User* DataBase::get_user(int fd) {
    if (fd_users.count(fd))
        return fd_users[fd];
    return nullptr;
}

void DataBase::set_user_fd(int fd, User* user) {
    fd_users[fd] = user;
}

void DataBase::remove_user_fd(int fd) {
    fd_users.erase(fd);
}

int DataBase::get_command_fd(int data_fd) {
    if (command_fds.count(data_fd))
        return command_fds[data_fd];
    return -1;
}

void DataBase::set_command_fd(int data_fd, int command_fd) {
    command_fds[data_fd] = command_fd;
}

void DataBase::remove_command_fd(int fd) {
    if (command_fds.count(fd)) {
        command_fds.erase(fd);
        close(fd);
    }
}