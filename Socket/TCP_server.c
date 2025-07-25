#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>             // for close()
int main() {

    char server_message[256] = "You have reach the server";

    // Create the server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = INADDR_ANY;  // Automatically resolve to any ip address in the machine

    // bind the socket to our ip and port
    bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(server_socket, 5);  // N is backlog: how many connection can be waiting essentialy for this perticular socket at one point in time

    // Create integer to hold the socket. Once we able to listen for socket
    // We can start accepting connection

    int client_socket = accept(server_socket, NULL, NULL);

    // send the message
    send(client_socket, server_message, sizeof(server_message), 0);

    return 0;
}