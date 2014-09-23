#!/bin/bash
# @author guanyu
#use d2u ./q1.sh to avoid problem in cygwin

src=q5.tcl
handle=../q2n3q5.awk
output=q5.txt
getData=../getData.awk
data=q5.data

if [ -e $output ]; then
   rm $output
fi

for (( snr=0; snr < 3; snr++))
do
   bin/lab6 2 $snr
done
echo

for (( snr=0; snr < 9; snr++))
do
   bin/lab6 8 $snr
done
echo

for (( snr=0; snr < 9; snr++))
do
   bin/lab6 16 $snr
done
echo

