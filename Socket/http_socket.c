#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <unistd.h>             /
#include <string.h>

int main() {

    // Open a file to serve
    FILE *html_data = fopen("index.html", "r");
    
    char response_data[1024];
    fgets(response_data, 1024, html_data);

    char http_header[2048] = "HTTP/1.1 200 OK\r\n\n"
    strcat(http_header, response_data);

    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Define address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8001);
    server_address.sin_addr.s_addr = INADDR_ANY;

    


    return 0;
}