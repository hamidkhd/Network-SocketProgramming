#include "ErrorHandler.hpp"

UserNotLoggin::UserNotLoggin() {
	message = USER_NOT_LOGGIN;
}

UserNotLoggin::~UserNotLoggin() {}

const char *UserNotLoggin::what() const throw() {
	return message.c_str();
}


WritingError::WritingError() {
	message = WRITING_ERROR;
}

WritingError::~WritingError() {}

const char *WritingError::what() const throw() {
	return message.c_str();
}


DefaultError::DefaultError() {
	message = DEFAULT_ERROR;
}

DefaultError::~DefaultError() {}

const char *DefaultError::what() const throw() {
	return message.c_str();
}


FullCapacity::FullCapacity() {
	message = FULL_CAPACITY;
}

FullCapacity::~FullCapacity() {}

const char *FullCapacity::what() const throw() {
	return message.c_str();
}


UserNotFound::UserNotFound() {
	message = USER_NOT_FOUND;
}

UserNotFound::~UserNotFound() {}

const char *UserNotFound::what() const throw() {
	return message.c_str();
}


IllegalAccess::IllegalAccess() {
	message = ILLEGAL_ACCESS;
}

IllegalAccess::~IllegalAccess() {}

const char *IllegalAccess::what() const throw() {
	return message.c_str();
}


SocketCreationFailed::SocketCreationFailed() {
	message = SOCKET_CREATION_FAILED;
}

SocketCreationFailed::~SocketCreationFailed() {}

const char *SocketCreationFailed::what() const throw() {
	return message.c_str();
}


AddressFailed::AddressFailed() {
	 	message = ADDRESS_FAILED;
}

AddressFailed::~AddressFailed() {}

const char *AddressFailed::what() const throw() {
	return message.c_str();
}


ConnectionFailed::ConnectionFailed() {
	message = CONNECTION_FAILED;
}

ConnectionFailed::~ConnectionFailed() {}

const char *ConnectionFailed::what() const throw() {
	return message.c_str();
}


SetSockOptFailed::SetSockOptFailed() {
	message = SET_SOCK_OPT__FAILED;
}

SetSockOptFailed::~SetSockOptFailed() {}

const char *SetSockOptFailed::what() const throw()
{
	return message.c_str();
}


BindFailed::BindFailed() {
	message = BIND_FAILED;
}

BindFailed::~BindFailed() {}

const char *BindFailed::what() const throw() {
	return message.c_str();
}


ListenFailed::ListenFailed() {
	message = LISTEN_FAILED;
}

ListenFailed::~ListenFailed() {}

const char *ListenFailed::what() const throw() {
	return message.c_str();
}


AcceptFailed::AcceptFailed() {
	message = LISTEN_FAILED;
}

AcceptFailed::~AcceptFailed() {}

const char *AcceptFailed::what() const throw() {
	return message.c_str();
}