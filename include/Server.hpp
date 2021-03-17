#ifndef _Server_
#define _Server_ 

#include "include.hpp"

#define PORT 8080 
#define IP_PROTOCOL 0 
#define IP_ADDRESS "127.0.0.1"


class Server {
    private:
        int server_socket;
        int new_socket;
        struct sockaddr_in serv_addr; 

    public:
        Server();
        ~Server();
        void initial_socket();
        void get_data_from_client();
};

#endif