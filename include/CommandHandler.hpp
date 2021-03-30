#ifndef _COMMAND_HANDLER_
#define _COMMAND_HANDLER_

#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <algorithm>
#include <sys/sendfile.h>

#include "include.hpp"
#include "DataBase.hpp"

#define DATA_PORT 8081 

class DataBase;


class CommandHandler {

    private:
        std::vector <std::string> input_words; 
        DataBase* data_base;
        int data_socket;

    public:
        CommandHandler();
        ~CommandHandler();
        std::string run_command_handler(std::string input, int client_fd);
        void separate_input_to_words(std::string input);
        std::string handle_command(int client_fd);
        int create_data_connection(int fd);
};

#endif 