#ifndef _COMMAND_HANDLER_
#define _COMMAND_HANDLER_

#include "Include.hpp"
#include "DataBase.hpp"
#include "Logger.hpp"

#define IP_PROTOCOL 0 
#define DATA_PORT 8081 

class DataBase;
class Logger;

class CommandHandler {
    
    private:
        DataBase* data_base;
        Logger* logger;
        int data_socket;
        struct sockaddr_in data_addr;
        std::vector <std::string> input_words; 

        void mkd(int client_fd, User* user);
        void dele(User* user);
        void ls(int client_fd, User* user);
        void cwd(User* user);
        void rename_command(User* user);
        void retr(int client_fd, User* user);
        void user_command(int client_fd, User* user);
        void pass_command(int client_fd, User* user);
        std::string help(User* user);
        void quit(int client_fd, User* user);

    public:
        CommandHandler(DataBase* _data_base, Logger* _logger);
        ~CommandHandler();
        std::string run_command_handler(std::string input, int client_fd);
        void separate_input_to_words(std::string input);
        std::string handle_command(int client_fd);
        int create_data_connection(int fd);
};

#endif 