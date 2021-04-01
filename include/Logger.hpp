#ifndef _LOGGER_
#define _LOGGER_ 

#include "Include.hpp"

class Logger {
    
    private:
        std::fstream log_file;
        std::time_t time;

    public:
        Logger();
        ~Logger();
        void find_time();
        void save_log(std::string massage);
};

#endif