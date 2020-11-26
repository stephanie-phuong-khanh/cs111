# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

default:
	gcc -lm -lmraa -Wall -Wextra lab4b.c -o lab4b

clean: 
	rm -f lab4b *.txt *.tar.gz

dist:
	tar -cvzf lab4b-604981556.tar.gz lab4b.c smoke_test.sh Makefile README

check: default
	chmod +x smoke_test.sh
	./smoke_test.sh
