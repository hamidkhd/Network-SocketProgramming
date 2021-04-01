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
		std::vector <std::string> accessible_files;
		std::string curr_dir;

	public:
		User(std::string _username, std::string _password, bool _admin_status, int _size, std::vector <std::string> _accessible_files);
		~User();
		std::string get_username();
		std::string get_password();
		bool get_admin_status();
		int get_size();
		void login(std::string pw);
		void logout();
		bool is_loggedin();
		std::string get_cwd();
		void set_cwd(std::string dir);
		bool check_accessiblility_file(std::string file_name);
};

#endif 