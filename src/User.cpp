#include "User.hpp"


User::User(std::string _username, std::string _password, std::string _admin_status, int _size, std::vector <std::string> _accessible_files) 
    : username(_username), password(_password), admin_status(_admin_status), size(_size), accessible_files(_accessible_files) {}

User::~User() {}

void User::check() {
    std::cout << "new user: " << std::endl;

    std::cout << this->username << std::endl;
    std::cout << this->password << std::endl;
    std::cout << this->admin_status << std::endl;
    std::cout << this->size << std::endl;

    for (long unsigned int i = 0; i < accessible_files.size(); i++) {
        std::cout << accessible_files[i] << std::endl;
    }
}