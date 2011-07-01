#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

static const int MAXPENDING = 5;

void HandleTCPClient(int clntSocket);

int main (int argc, char *argv[]) {

	if (argc != 2) {
		DieWithUserMessage("Parameter(s)", "<Server Port>");
	} // if (argc != 2) {

	in_port_t servPort = atoi(argv[1]);
	int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (servSock < 0) {
		DieWithSystemMessage("socket() failed\n");
	} // if (servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) {
	
	struct sockaddr_in servAddr;

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		DieWithSystemMessage("bind() failed");	
	} // if (bind(servSock, (struct socaddr *) &servAddr, sizeof(servAddr) < 0) {

	if (listen(servSock, MAXPENDING) < 0) {
		DieWithSystemMessage("listen() failed\n");
	} // if (listen(servSock, MAXPENDING) < 0) {

	for (;;) {	
		struct sockaddr_in clntAddr;
		socklen_t clntAddrLen = sizeof(clntAddr);

		int clntSock = accept(servSock, (struct sockaddr * ) &clntAddr, 
			&clntAddrLen);

		if (clntSock < 0) {
			DieWithSystemMessage("accept() failed\n");
		} // if (clntSock < 0) {

		char clntName[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, 
		  sizeof(clntName)) != NULL) {
			printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));	
		} // if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, 
		else {
			puts("Unable to get client address\n");
		} // else {
		
		HandleTCPClient(clntSock);	

		close(clntSock);
	} // for (;;) {	
	return 0;
} // int main (int argc, char *argv[]) {

void HandleTCPClient(int clntSocket) {

//	char buffer[BUFSIZE];

/* Seems to work without headers.
	Note that two \r\n are reqired betweeen the header and the html body

	char *html = "HTTP/1.0 200 OK\r\n"
//			"Server: WebServer 1.0\r\n"
			"Content-Type: text/html\r\n"
//			"Date: Thursday, 28 June 2011 18:55:30 PST\r\n"
//			"Content-Length: 28\r\n"
			"Connection: close\r\n\r\n"
			"<html><body>Hi</body></html>\r\n";
*/

//	int len = strlen(html);

	FILE *fp;
	long length;
	char *buf;
	fp = fopen("index.html", "r");
	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = (char *) malloc(length);
	fread(buf, length, 1, fp);
	fclose(fp);
/*	
	ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);

	if (numBytesRcvd < 0) {
		
		DieWithSystemMessage("recv() failed");
	} // if (numBytesRcvd < 0) {
*/

	ssize_t numBytesSent = send(clntSocket, buf, length, 0); 
	
	if (numBytesSent < 0) {
				  DieWithSystemMessage("send() failed");
	} //  if (numBytesSent < 0) {

	close(clntSocket);
	printf("closed\n");
} // void HandleTCPClient(int clntSocket) {

