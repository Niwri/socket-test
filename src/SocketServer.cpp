#include <iostream>
#include <winsock.h>

#define PORT 9909

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;

int main() {

	int nRet = 0;

	// Initialize WSA Variables for the library
	WSADATA ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0) { 
		std::cout << "WSA failed to initialize" << std::endl; 
		exit(errno); 
	} else std::cout << "WSA has initialized" << std::endl;

	// Initialize Socket using Stream Socket and TCP/IP
	int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (nSocket < 0) { 
		std::cout << "The socket has not opened" << std::endl; 
		exit(errno); 
	} else std::cout << "The socket opened successfully: " << nSocket << std::endl;
	
	// Initialize Environment for SockAddr Structure
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY; // Assigns system address of local machine. Can do inet_addr("127.0.0.1")
	memset(&(srv.sin_zero), 0, 8);

	// Bind Socket to Local Port
	nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));
	if (nRet < 0) {
		std::cout << "Failed to bind to local port" << std::endl; 
		exit(errno);
	} else std::cout << "Successfully bound to local port" << std::endl;

	// Listen Request from Client (Queues)
	nRet = listen(nSocket, 5);
	if (nRet < 0) {
		std::cout << "Failed to start listening" << std::endl;
		exit(errno);
	} else std::cout << "Started listening to requests" << std::endl;

	nMaxFd = nSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&fr);
	FD_ZERO(&fw);
	FD_ZERO(&fe);

	FD_SET(nSocket, &fr);
	FD_SET(nSocket, &fe);

	std::cout << "Before Select Call: " <<  fr.fd_count << std::endl;
	// Keep waiting for new requests and proceed as per requests
	nRet = select(nMaxFd + 1, &fr, &fw, &fe, &tv);
	if (nRet > 0) {
		// When Someone sends a request to socket over dedicated coonnection
	}
	else if (nRet == 0) std::cout << "Nothing on port: " << PORT << std::endl; 
	else std::cout << "Select requests failed" << std::endl;

	std::cout << "After Select Call: " << fr.fd_count << std::endl;
	
}