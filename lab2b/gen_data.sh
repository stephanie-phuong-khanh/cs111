#!/bin/bash

CSV="lab2b_list.csv"

[ -e $CSV ] && rm $CSV

# List-1: Throughput vs. thread count
# List-2: Mutex wait time vs. threads
for th in 1, 2, 4, 8, 12, 16, 24
do
    ./lab2_list --threads=$th --iterations=1000 --sync=m >> $CSV
    ./lab2_list --threads=$th --iterations=1000 --sync=s >> $CSV
done

# List-3: Successful iterations vs. threads
for th in 1, 4, 8, 12, 16
do
    for it in 1, 2, 4, 8, 16
    do
        ./lab2_list --threads=$th --iterations=$it --yield=id --lists=4 >> $CSV
    done
    for it in 10, 20, 40, 80
    do
        ./lab2_list --threads=$th --iterations=$it --sync=m --yield=id --lists=4 >> $CSV
        ./lab2_list --threads=$th --iterations=$it --sync=s --yield=id --lists=4 >> $CSV
    done
done

# List-4, List-5: Throughput vs. number of threads
for th in 1, 2, 4, 8, 12
do
    for li in 4, 8, 16
    do
        ./lab2_list --threads=$th --iterations=1000 --sync=m --lists=$li >> $CSV
        ./lab2_list --threads=$th --iterations=1000 --sync=s --lists=$li >> $CSV
    done
done
