#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc == 4) {
        printf("Usage: %s <peer_ip> <peer_port> <message>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Get some information about our peer from our args
    const char *peer_ip = argv[1];
    int peer_port = atoi(argv[2]);
    const char *message = argv[3];

    struct sockaddr_in peer_addr = {
        .sin_family = AF_INET,  // Address Family InterNet
        .sin_port = htons(peer_port), // Not all mechine use same bite order
    };

    if (inet_pton(AF_INET, peer_ip, &(peer_addr.sin_addr)) <= 0) { // Internet_Printable to Networh format
        perror("Something wrong withe the IP address\n");
        return EXIT_FAILURE;
    }

    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);  // SOCK_DGRAM:- Datagram
    if (udp_socket < 0) {
        perror("Sorry, couldn't create the socket.");
        return EXIT_FAILURE;
    }

    if (sendto(udp_socket, message, strlen(message) + 1, 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0) {
        close(udp_socket);
        return EXIT_FAILURE;
    }

    printf("Sent \"%s\" to %s:%d\n", message, peer_ip, peer_port);
    close(udp_socket);

    return EXIT_SUCCESS;
}