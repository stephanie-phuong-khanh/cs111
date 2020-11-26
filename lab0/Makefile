# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

default:
	gcc -g -Wall -Wextra lab0.c -o lab0

clean: 
	rm -f lab0 *.tar.gz

dist:
	tar -cvzf lab0-604981556.tar.gz lab0.c Makefile README backtrace.png breakpoint.png

check:	default file_redirection segfault catch invalid_argument invalid_input_file

file_redirection:
	echo "Hello world." > in.txt; \
	./lab0 --input in.txt --output out.txt; \
	cmp -s in.txt out.txt; \
	if [ $$? -eq 0 ]; then \
		echo "PASSED: simple file redirection"; \
	else \
		echo "FAILED simple file redirection"; \
	fi; \
	rm -f *.txt;

segfault:
	./lab0 --segfault --input in.txt --output out.txt; \
	if [ $$? -eq 139 ]; then \
		echo "PASSED: segfault without catch"; \
	else \
		echo "FAILED: segfault without catch"; \
	fi;

catch:
	./lab0 --segfault --catch; \
	if [ $$? -eq 4 ]; then \
		echo "PASSED: segfault with catch"; \
	else \
		echo "FAILED: segfault with catch"; \
	fi;

invalid_argument:
	./lab0 --invalid_argument; \
	if [ $$? -eq 1 ]; then \
		echo "PASSED: invalid argument"; \
	else \
		echo "FAILED: invalid argument"; \
	fi;

invalid_input_file:
	./lab0 --input nonexistent.txt; \
	if [ $$? -eq 2 ]; then \
		echo "PASSED: invalid input file"; \
	else \
		echo "FAILED: invalid input file"; \
	fi;
