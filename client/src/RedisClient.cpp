/*
 Establishing a TCP Connection to Redis (RedisClient)
    Uses Berkely sockers to open a TCP connection to the Redis server.
    Supports IPv4 and IPv6 resolution usnig getaddrinfo.

    Implements:
        connectToServer() -> Establishes the connection.
        sendCommand() -> Sends a command over the socket.
        disconnect() -> Close the socket when finished.
 */

#include "../include/RedisClient.h"
#include <netdb.h>
#include <string>
#include <sys/socket.h>

RedisClient::RedisClient(const std::string &host, int port)
    : host(host), port(port), sockfd(-1) {}

RedisClient::~RedisClient() { disconnect(); }

bool RedisClient::connectToServer() {
    struct addrinfo hints, *res = nullptr;
    std::memset(&hints, 0, sizeof(hints)); // Clear the hints structure
    hints.ai_family = AF_UNSPEC;           // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;       // Use TCP

    std::string portStr = std::to_string(port); // Convert port number to string
    int err = getaddrinfo(host.c_str(), portStr.c_str(), &hints,
                          &res); // Resolve address as linked list to res
    if (err != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(err)
                  << "\n"; // Print error message
        return false;      // Return false on failure
    }

    // Iterate through teh resolved addressses
    for (auto p = res; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol); // Create socket
        if (sockfd == -1)
            continue; // Skip if socket creation failed
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) ==
            0) {   // Attempt to connect
            break; // Break on successful connection
        }
        close(sockfd); // Close socket if connection failed
        sockfd = -1;   // Reset socket file descriptor
    }
    freeaddrinfo(res); // Free teh address information

    if (sockfd == -1) {
        std::cerr << "Could not connect to " << host << ":" << port
                  << "\n"; // Print failure message
        return false;
    }
    return true; // Return true on successful connection
}

void RedisClient::disconnect() {
    if (sockfd != -1) {
        close(sockfd); // Close socket if connection failed
        sockfd = -1;   // Reset socket file descriptor
    }
}
