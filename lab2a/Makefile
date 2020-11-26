# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

default:	add list
	
add:
	gcc -g -pthread -Wall -Wextra lab2_add.c -o lab2_add

list:
	gcc -g -pthread -Wall -Wextra lab2_list.c SortedList.c -o lab2_list

tests: default 
	rm -f lab2_add.csv lab2_list.csv
	touch lab2_add.csv lab2_list.csv
	# lab2_add
	./lab2_add --threads=1, --iterations=10, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=20, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=20, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=40, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=40, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=80, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=80, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=1000, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=1000, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10000, >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10000, --yield >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=1000, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=1000, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000, >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000, --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=1000, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=1000, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000, >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000, --yield >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=1000, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=1000, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000, >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000, --yield >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000, >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000, --yield >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100000 --yield >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=1000, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=1000, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=1000, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=1000, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=1000, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=1000, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000, --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000, --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --yield --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=1000 --yield --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10, --sync=m >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10, --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10, --sync=c >> lab2_add.csv
	./lab2_add --threads=1, --iterations=20, --sync=m >> lab2_add.csv
	./lab2_add --threads=1, --iterations=20, --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=20, --sync=c >> lab2_add.csv
	./lab2_add --threads=1, --iterations=40, --sync=m >> lab2_add.csv
	./lab2_add --threads=1, --iterations=40, --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=40, --sync=c >> lab2_add.csv
	./lab2_add --threads=1, --iterations=80, --sync=m >> lab2_add.csv
	./lab2_add --threads=1, --iterations=80, --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=80, --sync=c >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100, --sync=m >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100, --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=100, --sync=c >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=1, --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10, --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=20, --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=40, --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=80, --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=100, --sync=c >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=2, --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10, --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=20, --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=40, --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=80, --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=100, --sync=c >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=4, --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10, --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=20, --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=40, --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=80, --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=100, --sync=c >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=8, --iterations=10000 --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10, --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=20, --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=40, --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=80, --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=100, --sync=c >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=m >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=s >> lab2_add.csv
	./lab2_add --threads=12 --iterations=10000 --sync=c >> lab2_add.csv
	# lab2_list
	./lab2_list --threads=1 --iterations=10, >> lab2_list.csv
	./lab2_list --threads=1 --iterations=100, >> lab2_list.csv
	./lab2_list --threads=1 --iterations=1000, >> lab2_list.csv
	./lab2_list --threads=1 --iterations=10000, >> lab2_list.csv
	./lab2_list --threads=1 --iterations=20000 >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=10,100,1000 >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=10,100,1000 >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=10,100,1000 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=10,100,1000 >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=1, --yield=i >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=1, --yield=d >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=1, --yield=il >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=1, --yield=dl >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=2,4,8,16,32 >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=2,4,8,16,32 --yield=i >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=2,4,8,16,32 --yield=d >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=2,4,8,16,32 --yield=il >> lab2_list.csv
	-./lab2_list --threads=2, --iterations=2,4,8,16,32 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=1, --yield=i >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=1, --yield=d >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=1, --yield=il >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=1, --yield=dl >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=2,4,8,16,32 >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=2,4,8,16,32 --yield=i >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=2,4,8,16,32 --yield=d >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=2,4,8,16,32 --yield=il >> lab2_list.csv
	-./lab2_list --threads=4, --iterations=2,4,8,16,32 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=1, --yield=i >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=1, --yield=d >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=1, --yield=il >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=1, --yield=dl >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=2,4,8,16,32 >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=2,4,8,16,32 --yield=i >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=2,4,8,16,32 --yield=d >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=2,4,8,16,32 --yield=il >> lab2_list.csv
	-./lab2_list --threads=8, --iterations=2,4,8,16,32 --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1, >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1, --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1, --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1, --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=1, --yield=dl >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2,4,8,16,32 >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2,4,8,16,32 --yield=i >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2,4,8,16,32 --yield=d >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2,4,8,16,32 --yield=il >> lab2_list.csv
	-./lab2_list --threads=12 --iterations=2,4,8,16,32 --yield=dl >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1, --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=10,100,1000 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1, --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=10,100,1000 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1, --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=10,100,1000 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=1, --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=i --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=d --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=il --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=dl --sync=m >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=i --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=d --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=il --sync=s >> lab2_list.csv
	./lab2_list --threads=12 --iterations=10,100,1000 --yield=dl --sync=s >> lab2_list.csv
	./lab2_list --threads=1, --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=1, --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=2, --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=4, --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=8, --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=12, --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=12, --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=16, --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=16, --iterations=1000 --sync=s >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2_list.csv

graphs: tests
	./lab2_add.gp
	./lab2_list.gp

clean: 
	rm -f lab2_add lab2_list *.tar.gz

dist: graphs
	tar -cvzf lab2a-604981556.tar.gz lab2_add.c lab2_list.c SortedList.h SortedList.c \
	lab2_add.csv lab2_add-1.png lab2_add-2.png lab2_add-3.png lab2_add-4.png lab2_add-5.png \
	lab2_list.csv lab2_list-1.png lab2_list-2.png lab2_list-3.png lab2_list-4.png \
	lab2_add.gp lab2_list.gp Makefile README
