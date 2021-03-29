#ifndef _Server_
#define _Server_ 

#include "include.hpp"
#include "CommandHandler.hpp"

#define COMMAND_PORT 8080 
#define DATA_PORT 8081 
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1"

class CommandHandler;

class Server {
    private:
        int command_socket;
        int data_socket;
        struct sockaddr_in command_addr; 
        struct sockaddr_in data_addr; 

    public:
        Server();
        ~Server();
        void run();
        CommandHandler* command_handler;
        void initial_socket();
};

#endif