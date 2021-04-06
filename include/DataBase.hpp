#ifndef DATABASE
#define DATABASE 

#include "Include.hpp"
#include "User.hpp"

class User; 

class DataBase {

    private:
        std::ifstream json_file;
		std::vector <User*> users;
        std::map <int, User*> fd_users;
        std::map <int, int> command_fds;
        std::vector <std::string> restricted_files;
        int command_port;
        int data_port;

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
        int get_command_fd(int data_fd);
        void set_command_fd(int data_fd, int command_fd);
        void remove_command_fd(int fd);
        bool is_restricted(std::string fname);
        int get_command_port();
        int get_data_port();
};

#endif 