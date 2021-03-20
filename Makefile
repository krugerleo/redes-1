all:
	gcc client.c commands.c communication.c -o client -Wall -Wextra -pedantic -std=c99
	gcc server.c commands.c communication.c -o server -Wall -Wextra -pedantic -std=c99