#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>             // for close()

int main() {

    int network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Specify an address for a socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if(connection_status == -1) { printf("Error making a connection to remot server\n"); }

    // Receive data from the server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response), 0);

    printf("Rexponse: %s\n", server_response);

   // and then close the socket
    close(network_socket);

    return 0;
}