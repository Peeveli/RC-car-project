/*
 * ============================================================================
 * Name:        client.cpp
 * Part of:     RemoteCar project (C++ port)
 * Description: dirty C++ port of the client application
 * Version:     no idea i guess everything is 1.0
 * Original:	Rhadov
 * C++-port:	mikpa1337
 * License:     no
 * Created:     12/12/2019 --mikpa1337
 * Last edited: Fri Dec 13 01:31:51 2019 ~mikpa1337
 * ============================================================================
 */

// Include and link winsock cancer
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// TODO: Think
/*The Iphlpapi.h header file is required if an application is using the IP Helper APIs. */


// Include libraries TODO: Check if you can use C++ libraries instead
#include <stdio.h>
#include <conio.h>// INCREDIBLY deprecated, but need it for getch() so you dont have to press enter ....
#include <iostream>

// TODO: This probably shouldnt be hardcoded?
//#define DEFAULT_PORT "1337"

//#define DEFAULT_ADDRESS "127.0.0.1"

int main(int argc, char* argv[])
{
	// Check argument count ...
	
	// TODO REMOVE COMMENT
	if (argc != 3) { std::cout << "Usage: " << argv[0] << " IPaddress PORT\n"; return 1; }
	
	//if (argc != 3) { std::cout << "Usage: Give IP and PORT argument\n"; return 1; }
	// In ARGC 1 we have the program name
	// In ARGC 2 we have the IP
	// In ARGC 3 we have the PORT


	/*          Initialize winsock
				  X          X

			   X      XX       X
			   XX     XX      XX
			   XX            X
				XXX        XX
				  XXXXXXXXXX

	*/

	// Init winsock (Create a WSADATA object called wsaData.)
	WSAData wsaData;

	// Call WSAStartup and return its value as an integer and check for errors.
	int iResult = 0;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 3490;
	}

	/*
	    XXXXXX
	   XX    XXXXXXXXXXXXXX X
	   X            XX      XXXXX
	   XX           X      XXXX XX   XX   X      XX    XXXXXX  XXXXXX
	    XXXX        X      XXXXXXX   X    X     XXX   XXX      XX
	       XXX      X      XXXX     XX    X    X        XX      XXX
	         X      X      XX XX    XX   X    X          X        XXXX
	XX     XXX     XX      X   XX    XXXX     XX      X  X    XXXXXXXX
	 XXXXXXX                                    XXXXXXX  X    X
	*/

	// Declare an addrinfo object that contains a sockaddr structure
	struct addrinfo* result = NULL, * ptr = NULL, hints;

	// Initialize and load up struct with data
	ZeroMemory(&hints, sizeof(hints));	// Roughly the same as *nix "memset(&hints, 0, sizeof(hints));"
	hints.ai_family = AF_UNSPEC;		// IPv4 or IPv6? Either will do
	hints.ai_socktype = SOCK_STREAM;	// use TCP
	hints.ai_protocol = IPPROTO_TCP;	// TODO: what the hell is this

	// Resolve address and port. Error checking
	/*TODO: COMMENT OUT THE DEFAULT_ADDRESS ONE !!! !! IT IS FOR DEBUG PURPOSE !! */
	//iResult = getaddrinfo(argv[1], "1337", &hints, &result);
	iResult = getaddrinfo(argv[1], argv[2], &hints, &result);
	//iResult = getaddrinfo(DEFAULT_ADDRESS, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	/*
		XXXXXXX                                                          XXXXXXXXXXXXXXX
	  X              XXXXXXXXX         XXXXX      XX   XXX   XXXXXXXX         X
	  X            XX        XXX      XX          XXXXXX     XX               X
	  XXX          X           X     XX           XXXX       X                X
		XXXX       X           X     X            XXXX       XXXXXXXX         X
		   XXX     XX         XX     X            XX  X      X                X
			 X      XX       XX       XX     X    XX  XXX    X                X
	XXXXXXXXXX       XXXXXXXXX         XXXXXXX    XX    X    XXXXXXXX         X

	*/

	// TODO: create socket

	// SOCKET object
	SOCKET ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	// Basic error checking (making sure the socket is valid)
	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}
	// TODO: maybe more extensive error checking


	/*
	XXXXXXXX    XXXX
   X          XXX  XXX   X       X
   X        XX       XX XXX      X  XX     X   XXXXXX            XXXXXXXXXXX
   X        X         X X XX     X XXXX    X   X     X     XXXXX X  X
   X        X         X X  XX   XX X  X   XX  XX    XX   XX         X
   X       XX        XX X   X  XX X   XX XX   XXXXXX    XX          X
   XX      XX       XX XX   XX X  X    XXX    X         X           X
	XXX     XX     XX XX     XXX  X    XX     XXXX      XX          X
	  XXXXXX XXXXXXX  X       X                  XXXXX   XXXXXXX    X
	*/

	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// TODO: Should really try the next address returned by getaddrinfo if the connect call failed

	// Free the resources returned by getaddrinfo and print an error message
	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	/*
	XXXXXXXX         X  XXXXXX
	X      XXX     XXXX     XXXXXXXXXX
	X        X    XX  X      X
	X        X    X   XX     X          XXXX
	XX        X   XX XXXX     X        XX   X
	XX       XX  XXXXX  X     X       XX    XX
	XX      XX   X      X     X    XXXXXXXXXXXXX
	 X    XXX   XX      X     X     X        X
	XXXXXX     X       X     X     X        X
	*/

	// TODO: What is the buffer for the original C# application?
	/*According to the following microsoft link. its 8192 bytes*/
	/*https://docs.microsoft.com/en-us/dotnet/api/system.net.sockets.tcpclient.receivebuffersize?view=netframework-4.8*/
	#define DEFAULT_BUFLEN 8192

	// RECEIVING buffer
	int recvbuflen = DEFAULT_BUFLEN;


	// TODO: What are we sending?
	//const char* sendbuf = "this is a test"; // this is about 14 bytes 
	// HAS TO BE const
	
	// HARDCODED BUFFERS there is probably a better way to do this but this is just lazy
	const char* drive = "drive";
	const char* reverse = "reverse";
	const char* quit = "quit";
	const char* stop = "stop";

	const char* sendbuf = "this is a test";

	// i dont think we are receiving anything ... TODO some :thinking:
	char recvbuf[DEFAULT_BUFLEN];
	
	int yes = 1;	// some top notch coding right here


	/*WARNING: STUPID C++ CODE BELOW*/
	
	//iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	
	/*KAIKKI SAMALLA RIVILLÄ LOL*/
	//if (iResult == SOCKET_ERROR) { printf("send failed: %d\n", WSAGetLastError()); closesocket(ConnectSocket); WSACleanup(); return 1; }
	
	/*WARNING: STUPID C++ CODE ABOVE*/


	while (yes == 1)
	{
		// Clear console (???)
		system("cls");	// NOTE: this is a very big stupid way to do it theres got to be better way
						// like flushing the stream or something ?????
						// Why is this stupid? Its windows dependant, its unsafe.
		// cout some shit
		std::cout << "1: drive | 2: ??? | 3: stop | 4: quit\n";
		
		// strictly windows shit right here
		// conio.h allow getch() which lets you enter in some value without having to press enter
		char item = ' ';
		item = _getch();
		


		switch (item)
		{
		case '1':
			iResult = send(ConnectSocket, drive, (int)strlen(drive), 0);
			if (iResult == SOCKET_ERROR) { printf("send failed: %d\n", WSAGetLastError()); closesocket(ConnectSocket); WSACleanup(); return 1; }
			break;
		case '2':
			iResult = send(ConnectSocket, reverse, (int)strlen(reverse), 0);
			if (iResult == SOCKET_ERROR) { printf("send failed: %d\n", WSAGetLastError()); closesocket(ConnectSocket); WSACleanup(); return 1; }
			break;
		case '3':
			iResult = send(ConnectSocket, stop, (int)strlen(stop), 0);
			if (iResult == SOCKET_ERROR) { printf("send failed: %d\n", WSAGetLastError()); closesocket(ConnectSocket); WSACleanup(); return 1; }
			break;
		case '4':
			iResult = send(ConnectSocket, quit, (int)strlen(quit), 0);
			if (iResult == SOCKET_ERROR) { printf("send failed: %d\n", WSAGetLastError()); closesocket(ConnectSocket); WSACleanup(); return 1; }
			yes = 0;
			break;
		default:
			break;
		}
	}

	// IMPLEMENT: whatever the hell is equal to this 
	/*            // some top notch coding right here
            while (yes == true)
            {
                message = "stop";
                Console.Clear();
                Console.WriteLine("1: Drive | 2: Do some shit | 3: quit");
                int item;   // ???
                // What and how the hell do we do this in C++?
                bool conversion = int.TryParse(Console.ReadKey().KeyChar.ToString(), out item);

                // seems like an easy one
                if (conversion == true) {
                    switch (item)
                    {
                        case 1:
                            message = "drive";
                            break;
                        case 2:
                            message = "reverse";
                            break;
                        case 3:
                            message = "quit";
                            yes = false;
                            break;
                    }
                }
                //create a byte array the message
                int byteCount = Encoding.ASCII.GetByteCount(message);
                byte[] sendData = new byte[byteCount];
                sendData = Encoding.ASCII.GetBytes(message);
                //send the message
                NetworkStream stream = client.GetStream();
                stream.Write(sendData, 0, sendData.Length);*/


	/*
	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}*/

	
	// This seems like a debug feature
	printf("Bytes Sent: %ld\n", iResult);

	// TODO remove this and see what the fuck
	// shutdown the connection for sending since no more data will be sent
	// the client can still use the ConnectSocket for receiving data
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// It is very rude to close out a TCP session without saying that we received the opposite side's data
	
	// Receive data until the server closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (iResult > 0);
	/*
	XXXXXXXXX
	XX      XX     X
	X        X     X                                            XX
	X       XX     X       XX   XXXXXX     XXXXXXXXXX   XXX   X X XXX
	X  XXXXXX      XX    XXX    X         XX   XX      XX     XX    X
	XXXXXXX         XXXXXX     X                X      X       X XXXX
	XX    XXX        XX      XXXXXXXXXX         X      X       XXX
	XX     XX        X        X                 XX     XX      XX
	 XXXXXXX        XX       X                   X      XXXXXX XX
	XX              X        XXXXXXXXX           XX
					X
	*/

	// shutdown the send half of the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	// CLOSE SOCKET AND CLEAN UP!!
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;

}


/*				XXXXXXXXXXXXXXXXXXXX
	        XXXX                   XXXX
	     XXXX                         XXX
	    XX     XXXXXXXXXXX              XXX
	   XX     XX         X                XX
	  XX           X           XXXXXXXXXX   X
	 XX          XXXXX        XXXXXXXXXXX   XX
	 X           XXXXXX        XXXXXX  X     XX
	 X           XXXXXX        XXXXXX         X
	XX            XXX           XXXXX         X
	X                                         X
	X                                         X
	X           XXXX                          X
	X           X  XXXXXXXXXXXX               X
	X           X   XXXX   XXXXXX             X
	XX         XX    X     XXXXXXXXX         XX
	 XX      XXX   XXXXXXXXX    XXXX        XX
	  XX    XX              XXXXX           X
	   XXX XX              XX              XX
	     XXX              XX             XXX
	       X              X          XXXXX
	       XXXX           X     XXXXXX
	        XXXXXXX       XXXXXXX
	          XXXXXXX  XXXX
	              XXXXXX
*/
