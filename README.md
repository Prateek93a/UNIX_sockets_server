# 3 implementations of simple maths server using UNIX socket programming
The repository contains three different implementations for simple maths server. It also contains client side code. The function of the servers is to respond to the basic maths queries of the client. 
Sample query : `3+5`
Sample response : `8`
The purpose the project was to learn connection oriented UNIX socket programming.

## Server 1
This is a single threaded single process server that can handle requests from a single client at a time. When the first client connects, a dedicated socket is created for it and the server stops listening for further connections until the current client disconnects. If another client tries to connect at this time, it receives the message `Connection Busy`.  After the client disconnects, it starts listening for new connections again.


## Server 2
This is a multi process server that can handle requests from a multiple clients at the same time. Whenever a client connects, a dedicated socket is created for it and a new process is spawned to handle requests from this client. The server continues to listen for further connections. After the client disconnects, the process spawned exits as well.

## Server 3
This is a single process server that can handle requests from a multiple clients at the same time using the `select` system call. Whenever a client connects, a dedicated socket is created for it and is appended to the `current_sockets` set. Using the `select` system call, the server monitors which client has sent a query and generates the response accordingly. After the client disconnects, the corresponding socket is closed and removed from the `current_sockets` set.

## Client
This is a generic client side program. The client connects to the server and executes an infinite loop. Inside the loop, it takes input from the user, sends the input to the server and waits for the response. On receiving response it prints out the results.
 
