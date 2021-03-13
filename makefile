CC = g++ -std=c++11

all: result.out

result.out: main.o DataBase.o User.o 
	$(CC) main.o DataBase.o User.o -o result.out

main.o: main.cpp headers/include.hpp
	$(CC) -c main.cpp -o main.o

DataBase.o: DataBase.cpp headers/include.hpp
	$(CC) -c DataBase.cpp -o DataBase.o

User.o: User.cpp headers/include.hpp
	$(CC) -c User.cpp -o User.o

clean:
	rm *.o result.out main.o DataBase.o User.o 