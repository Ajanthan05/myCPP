#include <iostream>
#include <vector>
#include <WinSock2.h>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

/* FOR SERVER
    Initialize winsock library
    Create the socket
    Get IP and Port
    Bind the ip/port with the socket
    Listen on the socket
    Accept
    Reseve and send
*/

bool Initialize() {
    WSADATA data;
    return WSASTARTUP(MAKEWORD(2, 2), &data) == 0;
}

int main() {


    return 0;
}