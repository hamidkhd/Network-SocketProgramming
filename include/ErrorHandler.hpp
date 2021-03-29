#ifndef ERRORHANDLER
#define ERRORHANDLER

#include "include.hpp"

#define USER_NOT_LOGGIN "332: Need account for login."
#define WRITING_ERROR "501: Syntax error in parameters or arguments."
#define DEFAULT_ERROR "500: Error"
#define BAD_SEQUENCE "503: Bad sequence of commands"
#define FULL_CAPACITY "425: Can't open data connection."
#define USER_NOT_FOUND "430: Invalid username or password"
#define ILLEGAL_ACCESS "550: File unavailable."


#define SOCKET_CREATION_FAILED "Socket creation failed."
#define ADDRESS_FAILED "Invalid address."
#define CONNECTION_FAILED "Connection failed."
#define SET_SOCK_OPT__FAILED "Set sock opt failed."
#define BIND_FAILED "Bind failed."
#define LISTEN_FAILED "Listen failed."
#define ACCEPT_FAILED "Accept failed."
#define SEND_DATA_FAILED "Send data failed."
#define RECIVE_DATA_FAILED "Recive data failed."


class UserNotLoggin : public std::exception {
    private:
    	std::string message;

    public:
    	UserNotLoggin();
        ~UserNotLoggin();
    	const char *what() const throw();
};

class WritingError : public std::exception {
    private:
    	std::string message;

    public:
    	WritingError();
        ~WritingError();
    	const char *what() const throw();
};

class DefaultError : public std::exception {
    private:
    	std::string message;

    public:
    	DefaultError();
        ~DefaultError();
    	const char *what() const throw();
};

class FullCapacity : public std::exception {
    private:
    	std::string message;

    public:
    	FullCapacity();
        ~FullCapacity();
    	const char *what() const throw();
};

class UserNotFound : public std::exception {
    private:
    	std::string message;

    public:
    	UserNotFound();
        ~UserNotFound();
    	const char *what() const throw();
};

class BadSequence : public std::exception {
    private:
    	std::string message;

    public:
    	BadSequence();
        ~BadSequence();
    	const char *what() const throw();
};

class IllegalAccess : public std::exception {
    private:
    	std::string message;

    public:
    	IllegalAccess();
        ~IllegalAccess();
    	const char *what() const throw();
};

class SocketCreationFailed : public std::exception {
    private:
    	std::string message;

    public:
    	SocketCreationFailed();
        ~SocketCreationFailed();
    	const char *what() const throw();
};

class AddressFailed : public std::exception {
    private:
    	std::string message;

    public:
    	AddressFailed();
        ~AddressFailed();
    	const char *what() const throw();
};

class ConnectionFailed : public std::exception {
    private:
    	std::string message;

    public:
    	ConnectionFailed();
        ~ConnectionFailed();
    	const char *what() const throw();
};

class SetSockOptFailed : public std::exception {
    private:
    	std::string message;

    public:
    	SetSockOptFailed();
        ~SetSockOptFailed();
    	const char *what() const throw();
};

class BindFailed : public std::exception {
    private:
    	std::string message;

    public:
    	BindFailed();
        ~BindFailed();
    	const char *what() const throw();
};

class ListenFailed : public std::exception {
    private:
    	std::string message;

    public:
    	ListenFailed();
        ~ListenFailed();
    	const char *what() const throw();
};

class AcceptFailed : public std::exception {
    private:
    	std::string message;

    public:
    	AcceptFailed();
        ~AcceptFailed();
    	const char *what() const throw();
};

class SendDataFailed : public std::exception {
    private:
    	std::string message;

    public:
    	SendDataFailed();
        ~SendDataFailed();
    	const char *what() const throw();
};

class ReciveDataFailed : public std::exception {
    private:
    	std::string message;

    public:
    	ReciveDataFailed();
        ~ReciveDataFailed();
    	const char *what() const throw();
};

#endif