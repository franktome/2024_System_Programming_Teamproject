Line Tracer car using Raspberry Pi
=
-Run car kit using Raspberry Pi
-Implementation of line detection and movement using an infrared sensor
-QR recognition using camera sensor
-Receive map information and item information through socket communication with the main server
-Implementing an algorithm to get a lot of points

Game Environment
● Play time
○ About 2 minutes. (Might be changed)
● Map
○ 4X4 grid with 4 rows, 4 columns and 25 intersections.
○ (𝑥, 𝑦) 𝑤ℎ𝑒𝑟𝑒,  0 ≤ 𝑥, 𝑦 ≤ 4
○ See figure 1.
● QR code
○ QRcode blocks will be placed on every single intersection.
○ A QRcode gives you the location of the corresponding intersection.
○ You should detect and decode QRcode to reach the intersection.
○ A QRcode gives you “xy”.
■ For example, if you reach the intersection (1,4), you will get 14.
■ (0,0) ⇒ 00 (3,2) ⇒ 32
○ You have to convert “xy” to two integer values.
■ For example, when you get 14, convert it to int row =1, int col =4
■ You’re going to use these integers to communicate with a server.
○ In short, read the QR code and send the locational information to server. Then,
you “reach” the intersection.
● Progress
○ An intersection can have three statuses.
■ No item: Nothing will happen.
■ Item: Get score  
