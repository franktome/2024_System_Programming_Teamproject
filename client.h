#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <math.h>
#include <signal.h>
#include <wiringPi.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <wiringPiI2C.h>
#include <ctype.h>

#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define Node Node1
#include "qr_detect.h"
#undef Node

#define Node Node2
#include "server.h"
#undef Node


extern DGIST dgist;
extern client_info player;
extern int my_row;
extern int my_col;
extern client_info other_player;
extern int fd;
extern int track_signal[4];


void Valid_direction(int* dir, int* trap);
void decision_target(int* target, int* dis);
void* receiveData(void* arg);
void init();
void _close();
void write_block(unsigned char reg, unsigned char *command, int length);
void get_track_signal(int* track_signal);
void control(int l_dir, int l_speed, int r_dir, int r_speed);
void run(int speed1, int speed2);
void turn_right(int l_speed, int r_speed);
void turn_left(int l_speed, int r_speed);
void tracking_init1();
void tracking_init2(int signum);
void head_bang(int id, int angle);


#endif // CLIENT_H
