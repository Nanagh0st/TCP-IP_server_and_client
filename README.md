# TCP-IP_server_and_client
Simple TCP client-server (in C/C++ for Linux), which includes: 
1. a TCP server listening on a port (e.g.,8080); it could accept multiple clients, and echo back clients' messages 
2. a TCP client, which could: 
  a. read command line options for server IP/port 
  b. periodically (e.g., every second) send a message (e.g., a sent message count) to the server, and print out server's response 
