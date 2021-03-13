#include "headers/DataBase.hpp"

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
            this->users.push_back(*new User(username, password, admin_status, stoi(size), _accessible_files));
        else 
            this->users.push_back(*new User(username, password, admin_status, stoi(size), empty));
    }

}

void DataBase::check() {
    for (int i = 0; i < users.size(); i++) {
        users[i].check();
    }
}