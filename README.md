# FTP Server & Client

## Overview
This project was done as a computer assignment for Computer Networks course at University of Tehran. The aim of the project was to implement an FTP server and client using sockets in C++.

## Architecture
The server uses two sockets, one socket for receiving commands from clients and sending response (command socket), another one for sending files (data socket).

Server uses `select()` system call to handle requests from multiple clients without blocking the server process. For sending files from server, a new process is created using `fork()`.

Server reads some configurations from `config.json`. These configurations include a list of users and their passwords and data limits and admin status, ports used for two sockets, and a list of protected files that cannot be viewed by normal users.

## How to run
First the project should be compiled using make:
```
$ make
```
then server and client processes can be started:
```
$ ./server.out
```
```
$ ./client.out
```
