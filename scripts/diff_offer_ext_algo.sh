#!/bin/bash
file=$1
count=0
cat $file|while read line
do
   # part1=`echo $line|cut -d'--' -f1`
    #part2=`echo $line|cut -d'--' -f2`
    #part3=`echo $line|cut -d'--' -f3`
    part1=$(echo "$line" | awk -F'--' '{print $1}')
    part2=$(echo "$line" | awk -F'--' '{print $2}')
    part3=$(echo "$line" | awk -F'--' '{print $3}')

    #echo "Part 1: $part1"
    #echo "Part 2: $part2"
    #echo "Part 3: $part3"
    if [ "$part3" == "$part2" ];then
        echo "$part2 and $part3 is same"
        ((count++))
    else
        echo "not same $line"
    fi

done
echo "$count"