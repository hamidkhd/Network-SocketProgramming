#ifndef USER
#define USER 

#include "headers/include.hpp"

class User {

    private:
    	std::string username;
    	std::string password;
    	std::string admin_status;
        int size;
		std::vector <std::string> accessible_files;

	public:
		User(std::string _username, std::string _password, std::string _admin_status, int _size, std::vector <std::string> _accessible_files);
		~User();
		void check();
};

#endif 