#!/bin/bash


#grep meta_sample_info log/*|awk -F"meta_sample_info" '{print$2}'
cat $1|while read line
do
    echo "start" >> $3
    echo ${line} |`jq '.demandInfo[].demandOfferId'|sort |uniq -c >> $3`
    echo ${line} |`jq '.requestId'>> $3`
done