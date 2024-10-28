# Resources for assignment 3 
- C socket call guidance: https://www.ibm.com/docs/en/zos/3.1.0?topic=programming-c-socket-call-guidance

- Blocking and non-blocking IO: https://medium.com/coderscorner/tale-of-client-server-and-socket-a6ef54a74763



# Multi-Threaded Network Server for Pattern Analysis


## Overview

This project implements a multi-threaded network server capable of managing incoming connections, processing text data, and analyzing patterns within the data.

## Requirements

- GCC (GNU Compiler Collection)
- pthread library for threading support

## Running the Server

1. **Compile the program:**
   To compile the server, run: **make** 



2. **Start the server:**
To start the server, run: ./assignment3 -l <port_number> -p <string_pattern>
where port_number is any positive integer and string_patter is a string that will be searched in the client's text files.


3. **Start the client**
- Download netcat tool
- Follow the command: nc localhost <port_number> -i <delay> < file.txt
where delay is any positive integer, port_number is the port number specified by the server and file.txt could be any one of book1.txt, book2.txt, ..., book12.txt.


### Testing
I have attempted to test the server within the file Test_assignment3.c, however due to time constraints, it was not possible to do so. 





<b>Note:</b> This is a very elementary README file that I cooked up last-minute! Please let me know if there's anything unclear about the set-up.