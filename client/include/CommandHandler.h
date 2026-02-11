#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include <string>
#include <vector>

class CommandHandler {
  public:
    // Split command into tokens
    static std::vector<std::string> splitArgs(const std::string &input);
};

#endif // COMMAND_HANDLER_H
