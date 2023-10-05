#include <iostream>
#include <winsock.h>
#include <string.h>

#define PORT 9909

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;
int nClientSocket;

void socketExit() {
	WSACleanup();
	exit(errno);
}

/*
1. Set up library
2. Set up socket
3. Set up environment for socket address
4. Set up socket options for how to use addresses
5. Bind socket to a local port
6. Enable listening for socket
7. Reset file descriptors and set file descriptors to socket
8. Process incoming new requests into file descriptors
9. Loop back to 7.
*/

int main() {
	int nRet;

	// Initialize WSA Variables for the library
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) { 
		std::cout << "WSA failed to initialize" << std::endl; 
		exit(errno); 
	} else std::cout << "WSA has initialized" << std::endl;

	// Initialize Socket using Stream Socket and TCP/IP
	nClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Socket Descriptor
	if (nClientSocket < 0) { 
		std::cout << "The socket has not opened" << std::endl; 
		socketExit();
	} else std::cout << "The socket opened successfully: " << nClientSocket << std::endl;
	
	// Initialize Environment for SockAddr Structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(srv.sin_zero), 0, 8);
	
	// Bind Socket to Local Port
	nRet = connect(nClientSocket, (struct sockaddr*)&srv, sizeof(srv));
	if (nRet < 0) {
		std::cout << "Connect failed. Error Code: " << nRet << std::endl; 
		socketExit();
	} else {
        std::cout << "Connected to the client."  << std::endl;
		char buff[255];
		recv(nClientSocket, buff, 255, 0);
		std::cout << buff << std::endl;
		
		while(1) {
			fgets(buff, 256, stdin);
			send(nClientSocket, buff, 256, 0);
			recv(nClientSocket, buff, 256, 0);
			std::cout << buff << std::endl;
		}
    }
	 

	
}