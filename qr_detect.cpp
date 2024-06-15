#include<qr_detect.h>

QRCodeDetector detector;

const char* qr_code() {
    string data="";
    VideoCapture camera(0);
    if (!camera.isOpened()) {
        return data.c_str();
    }

    camera.set(CAP_PROP_FRAME_WIDTH, 320);
    camera.set(CAP_PROP_FRAME_HEIGHT, 240);
    camera.set(CAP_PROP_FPS, 120);
    camera.set(CAP_PROP_BRIGHTNESS, 40);
    camera.set(CAP_PROP_CONTRAST, 50);
    camera.set(CAP_PROP_EXPOSURE, 156);

    Mat frame;
    camera >> frame;
   
   
    ImageScanner scanner;
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    Mat gray_image;
    cvtColor(frame, gray_image, COLOR_BGR2GRAY);

    int width = gray_image.cols;
    int height = gray_image.rows;
    uchar *raw = (uchar *)(gray_image.data);

    Image zbarImage(width, height, "Y800", raw, width * height);
    scanner.scan(zbarImage);

    auto symbol=zbarImage.symbol_begin();
    if (symbol!=zbarImage.symbol_end()){data = symbol->get_data();}
    else {return data.c_str();}
   
    camera.release();
    destroyAllWindows();
    return data.c_str();
}

2024년 6월 15일 (토) 오후 12:55, 강소은 <balgarack12@gmail.com>님이 작성:
// client.c
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
#include <qr_detect.h>
#undef Node

#define Node Node2
#include "server.h"
#undef Node



#define client_num 0  // Assume client_num as 0 for demonstration

#define dev "/dev/i2c-1"

#define TRACKING_LEFT1 2
#define TRACKING_LEFT2 3
#define TRACKING_RIGHT1 0
#define TRACKING_RIGHT2 7

DGIST dgist;  
client_info player;
int my_row; // player2->4
int my_col; // player2->4
client_info other_player;



int fd;
int track_signal[4];

void Valid_direction(int* dir, int* trap) {
    for (int i = 0; i < 4; i++) {
        dir[i] = 1;
        trap[i] = -1;
    }

    player = dgist.players[client_num];
    //my_row = row;
    //my_col = col;
    //my_row = player.row;
    //my_col = player.col;


    if ((my_row + 1 >= MAP_ROW) || ((trap[0] = dgist.map[my_row + 1][my_col].item.status) == 2)) {
        dir[0] = 0;
    }// 하
    if ((my_row - 1 < 0) || ((trap[1] = dgist.map[my_row - 1][my_col].item.status) == 2)) {
        dir[1] = 0;
    }// 좌
    if ((my_col - 1 < 0) || ((trap[2] = dgist.map[my_row][my_col - 1].item.status) == 2)) {
        dir[2] = 0;
    }// 우
    if ((my_col + 1 >= MAP_COL) || ((trap[3] = dgist.map[my_row][my_col + 1].item.status) == 2)) {
        dir[3] = 0;
    }
    for(int i=0;i<4;i++){
printf("%d",dir[i]);
}

    if (dir[0] == 0 && dir[1] == 0 && dir[2] == 0 && dir[3] == 0) {
        printf("There is no way to avoid traps\n");
        for (int i = 0; i < 4; i++) {
            if (trap[i] == 2) dir[i] = 1;
        }
    }
}

void decision_target(int* target, int* dis){
    target[0]=my_row; target[1]=my_col;
    float max_value=0;
    float value;
    Item tem; int distance;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tem=dgist.map[i][j].item;
            //printf("item: %d\n",tem.score);
            //printf("item status: %d\n",tem.status);
            if ((tem.status==1)&&(tem.score>0)) {
                distance = abs(my_row-dgist.map[i][j].row) + abs(my_col-dgist.map[i][j].col);
                if (distance!=0) {
                    value=tem.score/(float)distance;
                    if(max_value<value){
                        max_value=value;
                        *dis=distance;
                        target[0]=dgist.map[i][j].row;
                        target[1]=dgist.map[i][j].col;
                    }
                }
            }
        }
    }
}

void* receiveData(void* arg) {
    int sock = *(int*)arg;

    while (1) {
        int valRead = read(sock, &dgist, sizeof(DGIST));
        if (valRead <= 0) {
            printf("Connection closed or error\n");
            close(sock);
            pthread_exit(NULL);
        }
        // Print the map and players information
        printMap(&dgist);
        printPlayer(&dgist);
    }
    return NULL;
}


//run
void init(){
    fd=open(dev, O_RDWR);
    ioctl(fd, I2C_SLAVE, 0X16);
}

void _close(){
    close(fd);
}

void write_block(unsigned char reg, unsigned char *command, int length){
    unsigned char* data=(unsigned char*)malloc(length+1);
    data[0]=reg;
    for (int i=0;i<length;i++){
        data[i+1]=command[i];
}
    write(fd, data, length+1);
    free(data);
}

void get_track_signal(int* track_signal) {
    track_signal[0] = digitalRead(TRACKING_LEFT1);
    track_signal[1] = digitalRead(TRACKING_LEFT2);
    track_signal[2] = digitalRead(TRACKING_RIGHT1);
    track_signal[3] = digitalRead(TRACKING_RIGHT2);
   
   
}
void control(int l_dir, int l_speed, int r_dir, int r_speed){
    unsigned char data[4]={l_dir, l_speed, r_dir, r_speed};
    write_block(0x01, data, 4);
}

void run(int speed1, int speed2){
    control(1, speed1, 1, speed2);
}


void turn_right(int l_speed, int r_speed){
    control(1, l_speed, 0, r_speed);
}

void turn_left(int l_speed, int r_speed){
    control(0, l_speed, 1, r_speed);
}


//run over


//tracking
void tracking_init1() {
   
    pinMode(TRACKING_LEFT1, INPUT);
    pinMode(TRACKING_LEFT2, INPUT);
    pinMode(TRACKING_RIGHT1, INPUT);
    pinMode(TRACKING_RIGHT2, INPUT);
}

void tracking_init2(int signum) {
   
    pinMode(TRACKING_LEFT1, INPUT);
    pinMode(TRACKING_LEFT2, INPUT);
    pinMode(TRACKING_RIGHT1, INPUT);
    pinMode(TRACKING_RIGHT2, INPUT);
    exit(0);
}


//tracking over

//head bang
void head_bang(int id, int angle){
    if (angle<0) angle=0;
    else if (angle>180) angle=180;
    unsigned char data[2]={id, angle};
    write_block(0x03, data, 2);
}

int main(int argc, char *argv[]) {
if (client_num==0){
my_row = 0; // player2->4
my_col = 0; // player2->4
}
else {
my_row = 4; // player2->4
my_col = 4; // player2->4
printf("here");
}

    int sock = 0;
    struct sockaddr_in serv_addr;
    pthread_t tid;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int port = atoi(argv[2]);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address or Address not supported\n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Connection Failed\n");
        return -1;
    }

    printf("Connected to the server\n");

int pthread_signal;
    // Create a thread to receive data from the server
    pthread_signal = pthread_create(&tid, NULL, receiveData, (void *)&sock);

    ClientAction cAction;
    cAction.action = (Action)0;
    if(client_num==0){
cAction.row = 0;
cAction.col = 0;
}
else{
cAction.row = 4;
cAction.col = 4;
}

   

    int valid[4];
    int traps[4];
   
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 하드웨어(바퀴) 초기설정
    init();
   
    //int weight[4]={-60,-40,40,60}; //50 -> 4,25      80 -60, 40
    //int speed=80;
   
    int weight[4]={-35,-25,25,35}; //50 -> 4,25      80 -60, 40
    int speed=50;
    int sum=0;
    int left_speed=0;
    int right_speed=0;
    // const char* result = qr_code();
   
    wiringPiSetup();
    tracking_init1();
   
    signal(SIGINT, tracking_init2);
   
    ////////////////////////////////////////////////////////////////////////////////////////////////


    int number=0;
    int count=0;

    ////////////////////////////////////////////////////////////////////////////////////////////////

    int previous_act; //player2-> 1
    if (client_num==0){
previous_act=3; //player2-> 1
}
else if(client_num==1){
previous_act=1;
}
    int current_act;
    int wheel_direction[4][4] = {
        {0,1,3,2}, // 이전이 상
        {1,0,2,3}, // 이전이 하
        {2,3,0,1}, // 이전이 좌
        {3,2,1,0} // 이전이 우
    };//0 직진, 1 후진, 2 좌회전, 3 우회전



while (1) {
while (!pthread_signal) {
// Ensure the latest data is received before making a decision
//usleep(100000); // Slight delay to ensure data is received



while(1){
count++;
if (count==550){const char* result = qr_code();printf("%s\n", result);}
if (count==350){const char* result = qr_code();printf("%s\n", result);}
//number++;
//if(number==50){head_bang(2, 75);}//result = qr_code();printf("%s\n", result);}
//if(number==100){head_bang(2, 100);number=0;}//result = qr_code();printf("%s\n", result);}
//line tracking
get_track_signal(track_signal);
//printf("%d %d %d %d \n", track_signal[0],track_signal[1],track_signal[2],track_signal[3]);
for (int i=0; i<4;i++){
sum=sum+weight[i]*track_signal[i];
}
left_speed=speed-sum;
right_speed=speed+sum;

if (left_speed>255) left_speed=255;
if (left_speed<0) left_speed=0;

if (right_speed>255) right_speed=255;
if (right_speed<0) right_speed=0;

run(left_speed, right_speed);
sum=0;
delay(10);
if (((track_signal[0]==0)&&(track_signal[1]==0)&&(track_signal[2]==0))||((track_signal[1]==0)&&(track_signal[2]==0)&&(track_signal[3]==0))){printf("intersection\n");count=0;break;}
}
run(0,0);
delay(100);


send(sock, &cAction, sizeof(ClientAction), client_num);
my_row=cAction.row;
my_col=cAction.col;
printf("Sent position: (%d, %d)\n", cAction.row, cAction.col);
printf("my location : %d %d\n", my_row, my_col);

Valid_direction(valid, traps);



/*
for(int i=0;i<4;i++){
printf("%d",valid[i]);
}
printf(" \n");
*/
int target[2];
int dis2target;
decision_target(target, &dis2target);
printf("target: (%d, %d)\n", target[0], target[1]);

int buff;
int next[2]={my_row,my_col};
int fixed=0;

if (valid[0]) {
buff = abs(my_row+1-target[0])+abs(my_col-target[1]);
if (buff < dis2target) {
current_act=0;
next[0] = my_row + 1;
next[1] = my_col;
fixed=1;
}
}
if (valid[1]&&(fixed==0)) {
buff = abs(my_row-1-target[0])+abs(my_col-target[1]);
if (buff < dis2target) {
current_act=1;
next[0] = my_row - 1;
next[1] = my_col;
fixed=1;
}
}
if (valid[2]&&(fixed==0) ) {
buff = abs(my_row-target[0])+abs(my_col-1-target[1]);
if (buff < dis2target) {
current_act=2;
next[0] = my_row;
next[1] = my_col - 1;
fixed=1;
}
}
if (valid[3]&&(fixed==0) ) {
buff = abs(my_row-target[0])+abs(my_col+1-target[1]);
if (buff < dis2target) {
current_act=3;
next[0] = my_row;
next[1] = my_col + 1;
fixed=1;
}
}
if (fixed==0) {
if (valid[0]==1) {
current_act=0;
next[0] = my_row + 1;
next[1] = my_col;
}
else if (valid[1]==1) {
current_act=1;
next[0] = my_row - 1;
next[1] = my_col;
}
else if (valid[2]==1) {
current_act=1;
next[0] = my_row;
next[1] = my_col - 1;
}
else if (valid[3]==1) {
current_act=1;
next[0] = my_row;
next[1] = my_col + 1;
}

}

cAction.row = next[0];
cAction.col = next[1];

cAction.action = (Action)0;
int a = (abs(other_player.row-my_row)+abs(other_player.col-my_col));
if ((a<=2)&&(player.bomb>0)){
cAction.action = setBomb;
}

//바퀴 방향전환 계산
int wheel_dir = wheel_direction[previous_act][current_act];
 
if(wheel_dir==1){ //후진 task1
printf("back\n");
turn_left(100,100);delay(1000);
}
else if(wheel_dir==2){ //좌회전 task2
printf("left\n");
turn_left(0,100);delay(1000);
}
else if(wheel_dir==3){ //우회전 task3
printf("right\n");
turn_right(100,0);delay(850);
}
else {
printf("straight\n");
run(50,50);
delay(500);}
run(0,0);
delay(100);
//cAction.row=isdigit(result[0])-'0';
//cAction.col=isdigit(result[2])-'0';



//send original location

//update previous_act
previous_act=current_act;


}
}
    run(0,0);
    delay(100);
    _close();

    close(sock);
    return 0;
}

// Implement printMap and printPlayer for client-side usage

void* printMap(void* arg) {
    DGIST* dgist = (DGIST*)arg;
    Item tmpItem;

    printf("==========PRINT MAP==========\n");
    for (int i = 0; i < MAP_ROW; i++) {
        for (int j = 0; j < MAP_COL; j++) {
            tmpItem = (dgist->map[i][j]).item;
            switch (tmpItem.status) {
                case nothing:
                    printf("- ");
                    break;
                case item:
                    printf("%d ", tmpItem.score);
                    break;
                case trap:
                    printf("x ");
                    break;
            }
        }
        printf("\n");
    }
    printf("==========PRINT DONE==========\n");
    return NULL;
}

void printPlayer(void* arg) {
    DGIST* dgist = (DGIST*)arg;
    client_info client;
    printf("==========PRINT PLAYERS==========\n");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client = dgist->players[i];
        printf("++++++++++Player %d++++++++++\n", i + 1);
        printf("Location: (%d, %d)\n", client.row, client.col);
        printf("Score: %d\n", client.score);
        printf("Bomb: %d\n", client.bomb);
    }
    printf("==========PRINT DONE==========\n");
}