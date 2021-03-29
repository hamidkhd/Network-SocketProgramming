#ifndef DATABASE
#define DATABASE 

#include "include.hpp"
#include "User.hpp"

class User; 

class DataBase {

    private:
		std::vector <User*> users;
        std::ifstream json_file;
        std::map <int, User*> fd_users;

    public:
        DataBase();
        ~DataBase();
        void readFile(std::string);
        void parse_json();
        std::vector <User*> get_users();
        User* get_user(std::string username);
        User* get_user(int fd);
        void set_user_fd(int fd, User* user);
        void remove_user_fd(int fd);
};

#endif 