#ifndef _Client_
#define _Client_ 

#include "include.hpp"

#define COMMAND_PORT 8080 
#define DATA_PORT 8081 
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1"


class Client {
    private:
        int command_socket;
        int data_socket;
        struct sockaddr_in command_addr; 
        struct sockaddr_in data_addr; 
        std::string input_command;

    public:
        Client();
        ~Client();
        void connect_to_server();
        std::string get_input_from_user();
        void send_data_to_server(std::string data);
        std::string recive_response_from_client();
};

#endif