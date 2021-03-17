#ifndef _Client_
#define _Client_ 

#include "include.hpp"

#define PORT 8080 
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1"


class Client {
    private:
        int sock_fd;
        struct sockaddr_in serv_addr; 

    public:
        Client();
        ~Client();
        void connect_to_server();
        void send_data_to_server(std::string data);
};

#endif