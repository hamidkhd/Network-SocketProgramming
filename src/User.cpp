#include "User.hpp"


User::User(std::string _username, std::string _password, bool _admin_status, int _size, std::vector <std::string> _accessible_files) 
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

bool User::get_admin_status() {
    return admin_status;
}

int User::get_size() {
    return size;
}

std::string User::get_cwd() {
    return curr_dir;
}

void User::set_cwd(std::string dir) {
    curr_dir = dir;
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

bool User::is_loggedin() {
    return authenticated;
}

bool User::check_accessiblility_file(std::string file_name) {
    if((*find(accessible_files.begin(), accessible_files.end(), file_name) == file_name) && admin_status == false)
        return false;
    return true;
}