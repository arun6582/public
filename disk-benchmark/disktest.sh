#!/bin/bash
echo "---------------------"
echo "Write Test Running. Please Wait..."
TESTFILE="tstfile"
write=$(dd if=/dev/zero bs=2048k of=$TESTFILE count=512 2>&1 | grep sec | awk '{print $1 / 1024 / 1024 / $5, "MB/sec" }')
purge
echo "Read Test Running. Please Wait..."
read=$(dd if=$TESTFILE bs=2048k of=/dev/null count=512 2>&1 | grep sec | awk '{print $1 / 1024 / 1024 / $5, "MB/sec" }')
echo "Write Speed is: $write"
echo "Read Speed is: $read"
echo "---------------------"
echo "Cleaning up. Please Wait..."
purge
rm $TESTFILE
exit 0
