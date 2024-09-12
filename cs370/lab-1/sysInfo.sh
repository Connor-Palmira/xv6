#!/bin/bash

#no args
if [ $# == 0 ]; then
    echo "Usage: sysInfo <sys|mem|disk>"
    exit 0
fi

#sys
if [ "$1" == "sys" ]; then
    #header
    echo "-------------------------------System Information----------------------------"
    #prints hostname
    echo -e "Hostname:\t\t$(hostname)"
    #prints uptime
    echo -e "uptime:\t\t\t$(uptime | awk '{print $3, $4}' | sed 's/,//')"
    #prints manufacturer
    echo -e "Manufacturer:\t\t$(cat "/sys/class/dmi/id/chassis_vendor")"
    #prints product name
    echo -e "Product Name:\t\t$(cat "/sys/class/dmi/id/product_name")"
    #prints product version
    echo -e "Version:\t\t$(cat "/sys/class/dmi/id/product_version")"
    #prints machine type
    echo -e "Machine Type:\t\t"$(vserver=$(lscpu | grep Hypervisor | wc -l); if [ $vserver -gt 0 ]; then echo 'VM'; else echo 'Physical'; fi)
    #prints operating system
    echo -e "Operating System:\t$(uname)"
    #prints kernel
    echo -e "Kernel:\t\t\t$(uname -r)"
    #prints architecture
    echo -e "Architecture:\t\t$(arch)"
    #prints processor name
    echo -e "Processor Name:\t\t$(awk -F':' '/^model name/ {print $2}' /proc/cpuinfo | uniq | sed -e 's/^[ \t]*//')"
    #prints active user
    echo -e "Active User:\t\t$USER"
    #prints ip address
    echo -e "Main System IP:\t\t"$(hostname -I)

#mem
elif [ "$1" == "mem" ]; then
    echo "-------------------------------CPU/Memory Usage------------------------------"
    echo "total used free shared buff/cache available"
    #prints free memory
    echo $(free | awk '/Mem:/')
    #prints free swap
    echo $(free | awk '/Swap:/')
    echo ""
    #prints memory usage
    echo -e "Memory Usage:    "$(free | awk '/Mem/{printf("%.2f%%"), $3/$2*100}')
    #prints swap usage
    echo -e "Swap Usage:      "$(free | awk '/Swap/{printf("%.2f%%"), $3/$2*100}')
    #prints cpu usage
    echo -e "CPU Usage:       "$(cat /proc/stat | awk '/cpu/{printf("%.2f%%\n"), ($2+$4)*100/($2+$4+$5)}' | awk '{print $0}' | head -1)

#disk
elif [ "$1" == "disk" ]; then
    echo "-------------------------------Disk Usage-------------------------------"
    echo $(df -h | awk '$NF=="/"{printf "Disk Usage: %s\t\t\n\n", $5}')
    echo ""
    echo $(df -Ph | sed s/%//g | awk '{ if($5 > 80) print $0;}')

#error
else
    echo "Error, invalid parameter"
fi

exit 0