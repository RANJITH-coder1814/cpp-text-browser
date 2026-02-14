#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
using namespace std;

#define PORT 80
#define BUFFER_SIZE 4096

int main() {
    string host;
    cout << "Enter website (example: example.com): ";
    cin >> host;

    // Resolve host
    struct hostent *server = gethostbyname(host.c_str());
    if (server == NULL) {
        cerr << "Error: No such host\n";
        return 1;
    }

    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error opening socket\n";
        return 1;
    }

    // Setup server address
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed\n";
        return 1;
    }

    // Create HTTP GET request
    string request = "GET / HTTP/1.1\r\nHost: " + host + "\r\nConnection: close\r\n\r\n";

    // Send request
    send(sock, request.c_str(), request.length(), 0);

    // Receive response
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    cout << "\n----- Website Content -----\n";

    while ((bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        cout << buffer;
    }

    cout << "\n----- End of Page -----\n";

    close(sock);
    return 0;
}
