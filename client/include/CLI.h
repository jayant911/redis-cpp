#ifndef CLI_H
#define CLI_H

#include <string>
//...
#include "CommandHandler.h"
#include "RedisClient.h"
#include "ResponseParser.h"

class CLI {
  public:
    CLI(const std::string &host, int port);
    void run();
    void executeCommand(const std::vector<std::string> &commandArgs);

    // handles pub-sub
    void handleSubscription(const std::vector<std::string> &commandArgs);

  private:
    RedisClient redisClient;
};

#endif // CLI_H
