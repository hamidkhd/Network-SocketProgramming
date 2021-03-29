#ifndef USER
#define USER 

#include "include.hpp"

class User {

    private:
    	std::string username;
    	std::string password;
    	std::string admin_status;
		bool authenticated;
        int size;
		std::vector <std::string> accessible_files;
		std::string curr_dir;

	public:
		User(std::string _username, std::string _password, std::string _admin_status, int _size, std::vector <std::string> _accessible_files);
		~User();
		std::string get_username();
		std::string get_password();
		std::string get_admin_status();
		int get_size();
		void login(std::string pw);
		void logout();
		std::string get_cwd();
		std::vector <std::string> get_accessible_files();
};

#endif 