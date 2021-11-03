all: client
CC = g++
CPPFLAGS = -c -Wall -Wextra -pedantic -I/opt/homebrew/opt/openssl@3/include
LDFLAGS = -lssl -lcrypto -lpthread -L/opt/homebrew/opt/openssl@3/lib     

client: Cmain.o
	$(CC) Cmain.o -o client $(LDFLAGS)

Cmain.o: Cmain.cpp
	$(CC) $(CPPFLAGS) Cmain.cpp

clean:
	rm *.o client