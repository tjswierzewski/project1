# Usage
$ ./client <-p port> <-s> [hostname] [Northeastern-username]

-s Turns on TLS connection

This project uses files from a Arbitrary Precision library found here:
https://github.com/arbitrary-precision/ap 

C++ files written by Tim Swierzewski:
- client.cpp
- evaluate.cpp
- check.cpp

The AP library files are loose in the root folder because Gradescope did not like them in their own folder.

For project 1 I started by creating a socket and seeing if it would connect to the host.
 Once this was confirmed I added TLS through the use of the OpenSSL library. Once I had this set up and confirmed it was operational
I was getting problems to solve. I wrote a few functions to solve the problems recursively and tried to answer the server.
After checking the non tls server in Wireshark I confirmed that the messages with just the header were sent before the server disconnected.
This meant that my evaluator was wrong and not the connection. The main issue I had was 64 bit integers were not big enough for the computations.
I added a lightweight library to add a type that allowed for extremely large integers. After a few implementation issues with the library my evaluator was fixed.
After running the the client I was able to retrieve the secret flag.

Debugging and testing was done through gdb and the check program.
Final checks were done with print statements and a python script to check each operation.
Due to the constraints of output for ./client the print statements did not make it in the final submission.