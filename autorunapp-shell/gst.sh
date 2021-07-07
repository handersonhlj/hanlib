#! /bin/bash
# process-monitor.sh
#process=$1

gst-launch-1.0 nvarguscamerasrc ! nvoverlaysink -e
