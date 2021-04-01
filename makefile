CC = g++
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
CFLAGS = -std=c++11 -Wall -Werror -I$(INCLUDE_DIR)

SERVER_EXECUTABLE_FILE = server.out
CLIENT_EXECUTABLE_FILE = client.out

all: $(BUILD_DIR) $(SERVER_EXECUTABLE_FILE) \
	$(BUILD_DIR) $(CLIENT_EXECUTABLE_FILE)



SERVER_OBJECTS = \
	$(BUILD_DIR)/DataBase.o \
	$(BUILD_DIR)/Logger.o \
	$(BUILD_DIR)/User.o \
	$(BUILD_DIR)/ErrorHandler.o \
	$(BUILD_DIR)/CommandHandler.o \
	$(BUILD_DIR)/Server.o \

CLIENT_OBJECTS = \
	$(BUILD_DIR)/ErrorHandler.o \
	$(BUILD_DIR)/Client.o \


DataBaseSensitivityList = \
	$(SRC_DIR)/DataBase.cpp \
	$(INCLUDE_DIR)/DataBase.hpp 

LoggerSensitivityList = \
	$(SRC_DIR)/Logger.cpp \
	$(INCLUDE_DIR)/Logger.hpp 

UserSensitivityList = \
	$(SRC_DIR)/User.cpp \
	$(INCLUDE_DIR)/User.hpp

ErrorHandlerSensitivityList = \
	$(SRC_DIR)/ErrorHandler.cpp \
	$(INCLUDE_DIR)/ErrorHandler.hpp

CommandHandlerSensitivityList = \
	$(SRC_DIR)/CommandHandler.cpp \
	$(INCLUDE_DIR)/CommandHandler.hpp

ServerSensitivityList = \
	$(SRC_DIR)/Server.cpp \
	$(INCLUDE_DIR)/Server.hpp

ClientSensitivityList = \
	$(SRC_DIR)/Client.cpp \
	$(INCLUDE_DIR)/Client.hpp



$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/DataBase.o: $(DataBaseSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/DataBase.cpp -o $(BUILD_DIR)/DataBase.o

$(BUILD_DIR)/Logger.o: $(LoggerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Logger.cpp -o $(BUILD_DIR)/Logger.o

$(BUILD_DIR)/User.o: $(UserSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/ErrorHandler.o: $(ErrorHandlerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ErrorHandler.cpp -o $(BUILD_DIR)/ErrorHandler.o

$(BUILD_DIR)/CommandHandler.o: $(CommandHandlerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/CommandHandler.cpp -o $(BUILD_DIR)/CommandHandler.o

$(BUILD_DIR)/Server.o: $(ServerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Server.cpp -o $(BUILD_DIR)/Server.o

$(BUILD_DIR)/Client.o: $(ClientSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Client.cpp -o $(BUILD_DIR)/Client.o


$(SERVER_EXECUTABLE_FILE): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) $(SERVER_OBJECTS) -o $(SERVER_EXECUTABLE_FILE)

$(CLIENT_EXECUTABLE_FILE): $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) $(CLIENT_OBJECTS) -o $(CLIENT_EXECUTABLE_FILE)



.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out