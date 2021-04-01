#include "DataBase.hpp"

using json = nlohmann::json;

DataBase::DataBase() {
    readFile("config.json");
    parse_json();
}

DataBase::~DataBase() {
    json_file.close();
}

void DataBase::readFile(std::string file_name) {
    json_file = std::ifstream(file_name);
}

void DataBase::parse_json() {

    nlohmann::json json_data;

    json_file >> json_data;


    for (auto& admin_file : json_data["files"]) {
        restricted_files.push_back(admin_file.get<std::string>());
    }

    for (auto& user_data : json_data["users"]) {
        std::string username = user_data["user"].get<std::string>();
        std::string password = user_data["password"].get<std::string>();
        std::string admin = user_data["admin"].get<std::string>();
        bool admin_status = true ? (admin == "true") : false;
        std::string size = user_data["size"].get<std::string>();

        if (admin_status == true)
            users.push_back(new User(username, password, admin_status, stoi(size)));
        else 
            users.push_back(new User(username, password, admin_status, stoi(size)));
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
        close(command_fds[fd]);
        command_fds.erase(fd);
    }
}

bool DataBase::is_restricted(std::string fname) {
    for (std::string name : restricted_files) {
        if (name == fname)
            return true;
    }
    return false;
}