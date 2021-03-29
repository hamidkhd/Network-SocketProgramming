#ifndef DATABASE
#define DATABASE 

#include "include.hpp"
#include "User.hpp"

class User; 

class DataBase {

    private:
		std::vector <User> users;
        std::ifstream json_file;

    public:
        DataBase();
        ~DataBase();
        void readFile(std::string);
        void parse_json();
        std::vector <User> get_users();
        //void check();
};

#endif 