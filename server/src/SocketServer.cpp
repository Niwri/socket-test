#include <iostream>
#include <winsock.h>

#define PORT 9909

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;
int nSocket;
int nArrClient[5];

void socketExit() {
	WSACleanup();
	exit(errno);
}

void processSend(int clientSocket) {
	char buff[256+1] = {0,};
	int nRet = recv(clientSocket, buff, 256, 0);
	if(nRet < 0) {
		std::cout << "Error receiving socket: " << clientSocket << std::endl;
		closesocket(clientSocket);
		for(int i = 0; i < 5; i++) {
			if(nArrClient[i] == clientSocket) {
				nArrClient[i] = 0;
				break;
			}
		}
	} else {
		std::cout << "Socket " << clientSocket << ": " << buff << std::endl;
		send(clientSocket, "Processed.", 11, 0);
	}
}

void processRequest() {
	if(FD_ISSET(nSocket, &fe)) {
		// Handle Exception

	} else if(FD_ISSET(nSocket, &fw)) {
		// Write to somethin.

	} else if(FD_ISSET(nSocket, &fr)) {
		// Read somethin
		int nLen = sizeof(struct sockaddr);
		int nClientSocket = accept(nSocket, NULL, &nLen); 
		if (nClientSocket > 0) {
			int nIndex = 0;
			for(nIndex = 0; nIndex < 5; nIndex++) 
				if(nArrClient[nIndex] == 0) {
					nArrClient[nIndex] = nClientSocket;
					send(nClientSocket, "Got connection", 255, 0);
					break;
				}
			if(nIndex == 5) {
				std::cout << "No space for new connection" << std::endl;
			}
		}
	} else {
		for(int i = 0; i < 5; i++) {
			if(FD_ISSET(nArrClient[i], &fr)) {
				processSend(nArrClient[i]);
			}
		}
	}
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
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Socket Descriptor
	if (nSocket < 0) { 
		std::cout << "The socket has not opened" << std::endl; 
		socketExit();
	} else std::cout << "The socket opened successfully: " << nSocket << std::endl;
	
	// Initialize Environment for SockAddr Structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY; // Assigns system address of local machine. Can do inet_addr("127.0.0.1")
	memset(&(srv.sin_zero), 0, 8);

	//SetSockOpt
	int nOptVal = 0;
	int nOptLen = sizeof(nOptVal);
	nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&nOptVal, nOptLen);
	if (nRet) {
		std::cout << "Setsockopt call failed. Error Code: " << nRet << std::endl;
		socketExit();
	} else std::cout << "Setsockopt call successful" << std::endl;

	// Bind Socket to Local Port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0) {
		std::cout << "Failed to bind to local port. Error Code: " << nRet << std::endl; 
		socketExit();
	} else std::cout << "Successfully bound to local port" << std::endl;

	// Listen Request from Client (Queues)
	nRet = listen(nSocket, 5);
	if (nRet < 0) {
		std::cout << "Failed to start listening. Error Code: " << nRet << std::endl;
		socketExit();
	} else std::cout << "Started listening to requests" << std::endl;

	nMaxFd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	while(true) {
		// Sets and re-sets FDs to socket after select call
		FD_ZERO(&fr);
		FD_ZERO(&fw);
		FD_ZERO(&fe);

		FD_SET(nSocket, &fr);
		FD_SET(nSocket, &fe);

		for(int i = 0; i < 5; i++) {
			if(nArrClient[i] != 0) {
				FD_SET(nArrClient[i], &fr);
				FD_SET(nArrClient[i], &fe);
			}
		}

		// Keep waiting for new requests and proceed as per requests
		nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv); 
		if (nRet > 0) {
			// When Someone sends a request to socket over dedicated coonnection
			std::cout << "Data on port. Processing." << std::endl;
			processRequest();
		} 
		else if (nRet == 0) std::cout << "Nothing on port: " << PORT << std::endl; 
		else  { 
			std::cout << "Select requests failed. Error Code: " << nRet << std::endl; 
			socketExit();
		}
	}
	
}