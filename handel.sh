#!/bin/bash
# @author guanyu
#use d2u ./q1.sh to avoid problem in cygwin

src=bin/lab6
#handle=../q2n3q5.awk
#output=ans
#getData=../getData.awk
#data=data

#if [ -e $output ]; then
#   rm $output
#fi

for modul in 2 8 16
do
   case $modul in
      2) MaxSNR=10;;
      8) MaxSNR=13;;
      16) MaxSNR=13;;
   esac
   case $modul in
      2) packet=BPSK;;
      8) packet=PSK8;;
      16) packet=QAM16;;
   esac
   rm -rf $packet/*.txt

#	echo ===========================================
#	echo  "Modulation: $modul"
#   for (( snr=0; snr < $MaxSNR; snr++))
#   do
#       echo -e "  $snr             \c"
#   done
#   echo

   for (( snr=0; snr < $MaxSNR; snr++))
   do
      $src $modul $snr -sound handel_.txt $packet/noCode$snr.txt
   done
#   echo

   for (( snr=0; snr < $MaxSNR; snr++ ))
   do
      $src $modul $snr -sound handel_.txt $packet/code$snr.txt -c
   done
#   echo

done	
#  for packetSize in 128 256 512
#  do
#	echo packetSize: $packetSize Byte
#	echo packetSize: $packetSize Byte >> $output

#  done
