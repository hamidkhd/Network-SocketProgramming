#ifndef _COMMAND_HANDLER_
#define _COMMAND_HANDLER_

#include "include.hpp"
#include "DataBase.hpp"

class DataBase;


class CommandHandler {

    private:
        std::vector <std::string> input_words; 
        DataBase* data_base;

    public:
        CommandHandler();
        ~CommandHandler();
        std::string run_command_handler(std::string input);
        void separate_input_to_words(std::string input);
        std::string handle_command();
        bool confirm_username(std::string username);
};

#endif 