all: tracker
CC = g++
CPPFLAGS = -c -Wall -Wextra -pedantic -I/opt/homebrew/opt/openssl@3/include
LDFLAGS = -L/opt/homebrew/opt/openssl@3/lib         

tracker: Tmain.o
	$(CC) Tmain.o -o tracker

Tmain.o: Tmain.cpp
	$(CC) $(CPPFLAGS) Tmain.cpp
	
clean:
	rm *.o tracker