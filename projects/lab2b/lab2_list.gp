#! /usr/bin/gnuplot
#
# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
#
# output:
#	lab2_list-1.png ... 
#	lab2_list-2.png ... 
#	lab2_list-3.png ... 
#	lab2_list-4.png ... 
#

# general plot parameters
set terminal png
set datafile separator ","

############################## LAB2_1.PNG ##############################
set title "List-1: Throughput vs. thread count"
set xlabel "Threads"
set logscale x 2
set xrange [0.8:32]
set ylabel "Throughput (ops/s)"
set logscale y 10
set output 'lab2b_1.png'
plot \
    "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
	    title 'mutex synchronized' with linespoints lc rgb 'red', \
    "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
	    title 'spin-lock synchronized' with linespoints lc rgb 'blue'

############################## LAB2_2.PNG ##############################
set title "List-2: Mutex wait time vs. threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.8:]
set ylabel "Time (ns)"
set logscale y 10
set output 'lab2b_2.png'
plot \
    "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	    title 'average time per operation' with linespoints lc rgb 'red', \
    "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	    title 'average wait-for-mutex time' with linespoints lc rgb 'blue'

############################## LAB2_3.PNG ##############################
set title "List-3: Successful iterations vs. threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.8:]
set ylabel "Successful iterations"
set logscale y 10
set output 'lab2b_3.png'
plot \
    "< grep 'list-id-none,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
        title 'no synchronization' with points lc rgb 'red', \
    "< grep 'list-id-m,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	    title 'mutex synchronization' with points lc rgb 'blue', \
    "< grep 'list-id-s,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	    title 'spin lock synchronization' with points lc rgb 'green'

############################## LAB2_4.PNG ##############################
set title "List-4: Aggregated throughput vs. number of threads (Mutex)"
set xlabel "Threads"
set logscale x 2
set xrange [0.8:]
set ylabel "Throughput (ops/s)"
set logscale y 10
set output 'lab2b_4.png'
plot \
    "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) title '1 list' with linespoints lc rgb 'blue', \
    "< grep 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/$7) title '4 lists' with linespoints lc rgb 'red', \
    "< grep 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/$7) title '8 lists' with linespoints lc rgb 'green', \
    "< grep 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/$7) title '16 lists' with linespoints lc rgb 'yellow'

############################## LAB2_4.PNG ##############################
set title "List-4: Aggregated throughput vs. number of threads (Spin lock)"
set xlabel "Threads"
set logscale x 2
set xrange [0.8:]
set ylabel "Throughput (ops/s)"
set logscale y 10
set output 'lab2b_5.png'
plot \
    "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) title '1 list' with linespoints lc rgb 'blue', \
    "< grep 'list-none-s,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/$7) title '4 lists' with linespoints lc rgb 'red', \
    "< grep 'list-none-s,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/$7) title '8 lists' with linespoints lc rgb 'green', \
    "< grep 'list-none-s,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/$7) title '16 lists' with linespoints lc rgb 'yellow'
