CC = g++
BUILD_DIR = build
SRC_DIR = src
INCLUDE_DIR = include
CFLAGS = -std=c++11 -Wall -Werror -I$(INCLUDE_DIR)

EXECUTABLE_FILE = result.out


OBJECTS = \
	$(BUILD_DIR)/Main.o \
	$(BUILD_DIR)/DataBase.o \
	$(BUILD_DIR)/User.o \
	$(BUILD_DIR)/ErrorHandler.o \
	$(BUILD_DIR)/Server.o \
	$(BUILD_DIR)/Client.o \


MainSensitivityList = \
	$(SRC_DIR)/Main.cpp \
	$(INCLUDE_DIR)/include.hpp

DataBaseSensitivityList = \
	$(SRC_DIR)/DataBase.cpp \
	$(INCLUDE_DIR)/DataBase.hpp 

UserSensitivityList = \
	$(SRC_DIR)/User.cpp \
	$(INCLUDE_DIR)/User.h

ErrorHandlerSensitivityList = \
	$(SRC_DIR)/ErrorHandler.cpp \
	$(INCLUDE_DIR)/ErrorHandler.h

ErrorHandlerSensitivityList = \
	$(SRC_DIR)/Server.cpp \
	$(INCLUDE_DIR)/Server.h

ErrorHandlerSensitivityList = \
	$(SRC_DIR)/Client.cpp \
	$(INCLUDE_DIR)/Client.h


all: $(BUILD_DIR) $(EXECUTABLE_FILE)


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/Main.o: $(CommandHandlerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Main.cpp -o $(BUILD_DIR)/Main.o

$(BUILD_DIR)/DataBase.o: $(CarwashSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/DataBase.cpp -o $(BUILD_DIR)/DataBase.o

$(BUILD_DIR)/User.o: $(StageSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/ErrorHandler.o: $(StageSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ErrorHandler.cpp -o $(BUILD_DIR)/ErrorHandler.o

$(BUILD_DIR)/Server.o: $(StageSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Server.cpp -o $(BUILD_DIR)/Server.o

$(BUILD_DIR)/Client.o: $(StageSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Client.cpp -o $(BUILD_DIR)/Client.o



$(EXECUTABLE_FILE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE_FILE)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out