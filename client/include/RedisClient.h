#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class RedisClient {
  public:
    RedisClient(const std::string &host, int port);
    ~RedisClient();

    bool connectToServer();
    void disconnect();
    int getSocketFD() const;
    std::string getHost() const;
    int getPort() const;

  private:
    std::string host;
    int port;
    int sockfd;
};

#endif // REDIS_CLIENT_H
