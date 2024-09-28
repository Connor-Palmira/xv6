#!/bin/bash

#no args
if [ $# == 0 ]; then
    echo "Usage: show <processName>"
    exit 0
fi

PID=$(pidof $1)

echo -e "Process $PID Information"
echo -e "voluntary_ctxt_switches: "$(grep '^voluntary_ctxt_switches' /proc/$PID/status | awk '{print $2}')
echo -e "nonvoluntary_ctxt_switches: "$(grep '^nonvoluntary_ctxt_switches' /proc/$PID/status | awk '{print $2}')
USRTIME=`echo "scale=2; $(cat /proc/$PID/stat | cut -d' ' -f14) / $(getconf CLK_TCK)" | bc`
echo -e "User Time: $USRTIME"
SYSTIME=`echo "scale=2; $(cat /proc/$PID/stat | cut -d' ' -f15) / $(getconf CLK_TCK)" | bc`
echo -e "System Time: $SYSTIME"
TOTTIME=$(echo $USRTIME + $SYSTIME | bc)
echo -e "Total Time: $TOTTIME"
echo -e "Start Time: "$(date +"%H:%M:%S")
