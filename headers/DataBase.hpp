#ifndef DATABASE
#define DATABASE 

#include "headers/include.hpp"

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
        void check();
};

#endif 