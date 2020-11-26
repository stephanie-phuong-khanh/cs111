# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

default:
	gcc -g -pthread -Wall -Wextra lab2_list.c SortedList.c -o lab2_list

tests: default 
	./gen_data.sh

graphs: tests
	./lab2_list.gp

profile:	default
	LD_PRELOAD=/usr/lib64/libprofiler.so \
	CPUPROFILE=raw.perf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text lab2_list raw.perf > profile.out
	printf '\n\n' >> profile.out
	pprof --list=thread_list lab2_list raw.perf >> profile.out
	rm -f raw.perf

clean: 
	rm -f lab2_list *.tar.gz

dist: graphs
	tar -cvzf lab2b-604981556.tar.gz lab2_list.c SortedList.h SortedList.c \
	lab2b_list.csv lab2b_1.png lab2b_2.png lab2b_3.png lab2b_4.png lab2b_5.png \
	lab2_list.gp gen_data.sh profile.out Makefile README
