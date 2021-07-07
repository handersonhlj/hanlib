#include <time.h>
#include <stdio.h>
#include <sys/time.h>
int main(void){
    time_t t,t1;   
    t = time(NULL);  
    sleep(10);
    t1 = time(NULL);
    printf("end = %ld\n",t1-t);

}