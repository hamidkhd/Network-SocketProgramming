#ifndef _USER_
#define _USER_ 

#include "Include.hpp"

class User {

    private:
    	std::string username;
    	std::string password;
    	bool admin_status;
		bool authenticated;
        int size;
		std::string curr_dir;

	public:
		User(std::string _username, std::string _password, bool _admin_status, int _size);
		~User();
		std::string get_username();
		std::string get_password();
		bool is_admin();
		int get_size();
		void login(std::string pw);
		void logout();
		bool is_loggedin();
		std::string get_cwd();
		void set_cwd(std::string dir);
};

#endif 