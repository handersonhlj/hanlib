#!/bin/bash
echo 0 > /sys/class/pwm/pwmchip0/export 
echo 10000000 > /sys/class/pwm/pwmchip0/pwm0/period
echo 2000000 > /sys/class/pwm/pwmchip0/pwm0/duty_cycle 
echo 1 > /sys/class/pwm/pwmchip0/pwm0/enable 

