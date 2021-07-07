#! /bin/bash
# process-monitor.sh
#process=$1
i=0
while true; do
	let i++
echo '--------------' $i 'time ----------------' 
#nohup gst-launch-1.0 nvarguscamerasrc ! nvoverlaysink -e
/home/h/gst.sh & 
sleep 5
pid=$(ps -ef | grep 'gst-launch-1.0' | grep -v grep | awk '{print $2}')
echo $pid
kill -SIGINT $pid
sleep 5 
done
