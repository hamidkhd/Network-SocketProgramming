#include "User.hpp"


User::User(std::string _username, std::string _password, std::string _admin_status, int _size, std::vector <std::string> _accessible_files) 
    : username(_username), password(_password), admin_status(_admin_status), size(_size), accessible_files(_accessible_files) {
        authenticated = false;
        curr_dir = getenv("PWD");
    }

User::~User() {}

std::string User::get_username() {
    return username;
}

std::string User::get_password() {
    return password;
}

std::string User::get_admin_status() {
    return admin_status;
}

int User::get_size() {
    return size;
}

std::string User::get_cwd() {
    return curr_dir;
}

void User::login(std::string pw) {
    if (pw == password)
        authenticated = true;
    else
        throw UserNotFound();
}

void User::logout() {
    if (!authenticated)
        throw BadSequence();
    authenticated = false;
}

std::vector <std::string> User::get_accessible_files() {
    return accessible_files;
}

//void User::check() {
//    std::cout << "new user: " << std::endl;
//
//    std::cout << this->username << std::endl;
//    std::cout << this->password << std::endl;
//    std::cout << this->admin_status << std::endl;
//    std::cout << this->size << std::endl;
//
//    for (long unsigned int i = 0; i < accessible_files.size(); i++) {
//        std::cout << accessible_files[i] << std::endl;
//    }
//}