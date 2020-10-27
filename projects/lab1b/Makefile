# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

default:	client server

client:
	gcc -g -lz -Wall -Wextra lab1b-client.c -o lab1b-client

server:
	gcc -g -lz -Wall -Wextra lab1b-server.c -o lab1b-server

clean: 
	rm -f lab1b-client lab1b-server *.tar.gz

dist:
	tar -cvzf lab1b-604981556.tar.gz lab1b-client.c lab1b-server.c Makefile README
