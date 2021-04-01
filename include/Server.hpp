#ifndef _SERVER_
#define _SERVER_ 

#include "Include.hpp"
#include "DataBase.hpp"
#include "CommandHandler.hpp"
#include "Logger.hpp"

#define COMMAND_PORT 8080 
#define DATA_PORT 8081 

#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1"

class DataBase;
class Logger;
class CommandHandler;

class Server {

    private:
        int command_socket;
        struct sockaddr_in command_addr; 
        
        DataBase* data_base;
        Logger* logger;
        CommandHandler* command_handler;

    public:
        Server();
        ~Server();
        void run();
        void initial_socket();
};

#endif