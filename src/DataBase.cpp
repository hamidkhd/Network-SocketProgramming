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

//void DataBase::check() {
//    for (long unsigned int i = 0; i < users.size(); i++) {
//        users[i].check();
//    }
//}