#! /bin/sh
file=$1
while [ "true" ]
do
  sudo python ps_mem.py  | grep "testThreading" >> $file
  sleep 1
done
