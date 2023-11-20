# ClientProgram-API
Communication across the Internet with the server.

Your task will be to program a "client program" capable of sending and receiving text messages.
In the first phase, you will be tasked with connecting to the server and identifying yourself with your personal number. 
Next, you need to be able to receive the message from the server and display it on the screen. 
If you succeed, Morpheus will address you on your own behalf and give you the tasks you need to complete.

BASIC VIEW OF THE ASSIGNMENT

Connect to the server with the IP address 147.175.115.34 on port 777 using the TCP protocol.
Send an introductory message (view it in the console).
Receive a reply (view it in the console).
Follow other commands received in communication with the server. 
Communication works in such a way that always after the server sends you something, it expects a response from you.

TIPS, TRICKS AND CONDITIONS

1. Load input from both the console and the spaces
By default, the scanf () function stops you from reading a string from an input after the first space occurs.
To save input, including spaces, it's best to use the fgets () function:

char name [1024];
int size = 1024;
fgets (name, size, stdin);
printf (“% s \ n”, name);

2. Work with received data

From a socket perspective, data is received only by a sequence of bytes, not strings. The received data may not even have a terminating zero (you all know what a terminated string is, right?). So how do you find out when the data that the server sends you in our case "ends"? Most messages end with a ‘\ n’ character. To avoid any clutter in the console when listing received data, you must replace this character with a trailing zero of ‘\ 0’. You can use this function, for example, to do this. You locate the character ‘\ n’ and replace it with a trailing zero.
However, in one specific case, you won't be able to rely on the ‘\ n 'character either. Then you will have to place the terminating zero manually based on the information that will be delivered to you on time.
