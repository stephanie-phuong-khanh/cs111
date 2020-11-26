#!/bin/bash

# NAME: Stephanie Doan
# EMAIL: stephaniekdoan@ucla.edu
# ID: 604981556

pass=true

./lab4b --log="LOGFILE" <<-EOF
SCALE=C
PERIOD=3
STOP
START
PERIOD=1
LOG hello world
OFF
EOF

ret=$?
if [ $ret -ne 0 ]
then
    pass=false
    echo "Test failed: returns incorrect value $ret"
fi

missing_commands=0
if [ ! -s LOGFILE ]
then
    pass=false
    echo "Test failed: did not create log file"
else
    for x in SCALE=C PERIOD=3 STOP START PERIOD=1 "LOG hello world" OFF SHUTDOWN
    do
        grep "$x" LOGFILE
        if [ $? -ne 0 ]
        then 
            pass=false
            echo "Test failed: did not log $x"
            missing_commands+=1
        fi
    done
fi

if [ "$pass" = true ]
then
    echo "Smoke test passed"
fi

rm -rf "LOGFILE"
