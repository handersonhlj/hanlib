#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define I2C_SLAVE_ADDR 0x48
#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24
#define CTRL_REG6 0x25

#define CTRL_REFERENCE 0x26

#define FIFO_CTRL_REG 0x2E

#define INT1_SRC 0x31  // read only
#define INT1_CFG 0x30
#define INT1_THS 0x32
#define INT1_DURATION 0x33

#define I2C_FILE_NAME "/dev/i2c-6"

int fd_gpio194_value;
int i2c_file;

static int set_i2c_register(int file, unsigned char addr, unsigned char reg, unsigned char value) {
  unsigned char outbuf[2];
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[1];
  messages[0].addr = addr;
  messages[0].flags = 0;
  messages[0].len = sizeof(outbuf);
  messages[0].buf = outbuf;
  outbuf[0] = reg;
  outbuf[1] = value;
  packets.msgs = messages;
  packets.nmsgs = 1;
  if (ioctl(file, I2C_RDWR, &packets) < 0) {
    perror("Unable to send data");
    return 1;
  }
  return 0;
}

static int get_i2c_register(int file, unsigned char addr, unsigned char reg, unsigned char *val) {
  unsigned char inbuf, outbuf;
  struct i2c_rdwr_ioctl_data packets;
  struct i2c_msg messages[2];
  outbuf = reg;
  messages[0].addr = addr;
  messages[0].flags = 0;
  messages[0].len = sizeof(outbuf);
  messages[0].buf = &outbuf;
  /* The data will get returned in this structure */
  messages[1].addr = addr;
  messages[1].flags = I2C_M_RD /* | I2C_M_NOSTART*/;
  messages[1].len = sizeof(inbuf);
  messages[1].buf = &inbuf;
  /* Send the request to the kernel and get the result back */
  packets.msgs = messages;
  packets.nmsgs = 2;
  if (ioctl(file, I2C_RDWR, &packets) < 0) {
    perror("Unable to send data");
    return 1;
  }
  *val = inbuf;
  return 0;
}

void read_register(void){
  unsigned char value;
  unsigned char addr = 0x20;
  for(int i=0;i<8;i++){
    value =0;
    get_i2c_register(i2c_file, I2C_SLAVE_ADDR, addr, &value);
    addr++;
    printf("CTRL_REG%d = %x\n",i+1, value);
  }
  addr = 0x2e;
    for(int i=0;i<8;i++){
    value =0;
    get_i2c_register(i2c_file, I2C_SLAVE_ADDR, addr, &value);
    addr++;
    printf("CTRL_REG-0x%x = %x\n",i+0x2e, value);
  }
}

void reset_lis2h(void){
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, CTRL_REG1, 0);  // 100hz
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, CTRL_REG2, 0);  //高通滤波器
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, CTRL_REG3, 0);  // INT1--FIFO overrun
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, CTRL_REG4, 0);  // +-2g
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, CTRL_REG5, 0);  // FIFO enable
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, CTRL_REG6, 0);  //高通滤波器
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, FIFO_CTRL_REG, 0);  // FIFO mode selection
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, INT1_CFG, 0);  // FIFO mode selection
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, INT1_THS, 0);  // FIFO mode selection
  set_i2c_register(i2c_file, I2C_SLAVE_ADDR, INT1_DURATION, 0);  // FIFO mode selection
}



int main(int argc, char **argv) {
  
int val;
int i,j;

  if ((i2c_file = open(I2C_FILE_NAME, O_RDWR)) < 0) exit(1);
  // read_register(); //compare the setted register;

  // set_i2c_register(i2c_file, I2C_SLAVE_ADDR, 0xef, 0);  

  for(j=0;j<1;j++){
		set_i2c_register(i2c_file, I2C_SLAVE_ADDR,0xef,j);
		printf("bank %d\n",j);
		usleep(500);
		
		for(i=0;i<=0xff;i++){
			val = 0;
      get_i2c_register(i2c_file, I2C_SLAVE_ADDR, i, &val);
		  usleep(100);
			printf("addr %2x = 0x%2X\n",i,val);
		}
	}
  close(i2c_file);
  return 0;
}
