Line Tracing car using Raspberry Pi
=
**2024 System Programming team project**

**1. Team name: 0123**

**2. Member name: 202011003 강승수, 202011133 윤현서, 202111003 강소은, 202211130 윤신이**

**3. Project period: 2024.05.20.~2024.06.02.**

**4. Demo Video(demo_video.mp4)**

- This is a video of a car doing line tracing and qr detect.

**5. Compilation method**

1. Type “make” in the command window.

2. (client code) ./client (IP address) (port number)
  ex) ./client 127.0.0.1 5050

3. (server code) ./server (port number)
  ex) ./server 5050

**6. Code content**

1. Run car kit using Raspberry Pi

2. Implementation of line detection and movement using an infrared sensor

3. QR recognition using camera sensor

4. Receive map information and item information through socket communication with the main server

5. Implementing an algorithm to get a lot of points

![car_image](https://github.com/franktome/2024_System_Programming_teamproject/assets/154505487/bdc918f0-b50d-460d-82ea-1e9356ce50e0)

![gamerule](https://github.com/franktome/2024_System_Programming_teamproject/assets/154505487/e80a7403-eb06-4e39-b83c-d102b8a35c1b)
