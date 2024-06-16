Line Tracing car using RaspberryPi
=
**2024 System Programming team project**

**1. Project introduction**
-
- Implement rasbot to perform line tracing and QR detection using Raspberry Pi.
- The ultimate goal is to implement an algorithm that receives map information and score information through socket communication with the server and obtains more scores than the opponent.

- Team name: 0123

- Member name: 202011003 강승수, 202011133 윤현서, 202111003 강소은, 202211130 윤신이

- Project period: 2024.05.20.~2024.06.02.



**2. Development Stack**
-
- raspberrypi4(2GB)
- C
- C++


**3. Demo Video(demo_video.mp4)**
-
- This is a video of a car doing line tracing and qr detect.

**4. Compilation method**
-
1. Type “make” in the command window.

2. (client code) ./client (IP address) (port number)
  ex) ./client 127.0.0.1 5050

3. (server code) ./server (port number)
  ex) ./server 5050

**5. Main function**
-
- Run car kit using Raspberry Pi

- Implementation of line detection and movement using an infrared sensor

- QR recognition using camera sensor

- Receive map information and item information through socket communication with the main server

- Implementing an algorithm to get a lot of points

**6. Code content**
-
- qr_detect.cpp: The qr_detect function is defined. The openCV library was used.
  
- qr_detect.h: This is the header file of qr_detect.cpp.
  
- client.c: Line tracing is performed according to the algorithm based on the coordinates obtained with the qr detect function. At this time, map information and score information were received through socket communication with the server, and the highest score was sent to the target point.
  
- client.h: This is the header file of client.c.
  
- server.c: This is the server code, which creates a map and randomly generates a score for each node. It manages information on game participants and sends map information and score information to participants through socket communication.
  
- server.h: This is the header file of server.c.

  


![car_image](https://github.com/franktome/2024_System_Programming_teamproject/assets/154505487/bdc918f0-b50d-460d-82ea-1e9356ce50e0)

![gamerule](https://github.com/franktome/2024_System_Programming_teamproject/assets/154505487/e80a7403-eb06-4e39-b83c-d102b8a35c1b)
