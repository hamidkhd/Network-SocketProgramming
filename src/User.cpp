#include "User.hpp"


User::User(std::string _username, std::string _password, bool _admin_status, int _size) 
    : username(_username), password(_password), admin_status(_admin_status) {
        authenticated = false;
        curr_dir = getenv("PWD");
        size_limit = _size * 1024;
    }

User::~User() {}

std::string User::get_username() {
    return username;
}

std::string User::get_password() {
    return password;
}

bool User::is_admin() {
    return admin_status;
}

std::string User::get_cwd() {
    return curr_dir;
}

void User::set_cwd(std::string dir) {
    char tmp[300];
    realpath(dir.c_str(), tmp);
    curr_dir = std::string(tmp);
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

bool User::can_download(uint size) {
    return size_limit >= size;
}

void User::subtract_size(uint size) {
    size_limit -= size;
}