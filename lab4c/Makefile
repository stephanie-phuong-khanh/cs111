# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

default: tcp tls

tcp:
	gcc -lm -lmraa -Wall -Wextra -g -o lab4c_tcp lab4c_tcp.c 

tls:
	gcc -lm -lmraa -Wall -Wextra -g -o lab4c_tls lab4c_tls.c -lssl -lcrypto

clean:
	rm -f lab4c_tcp lab4c_tls *.tar.gz

dist:
	tar -cvzf lab4c-604981556.tar.gz lab4c_tcp.c lab4c_tls.c Makefile README
