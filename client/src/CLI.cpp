#include "../include/CLI.h"
#include <vector>
// Simple helper to trim whitespace
static std::string trim(const std::string &str) {
    const std::string whitespace = "\t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);

    if (start == std::string::npos) {
        return "";
    }

    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

CLI::CLI(const std::string &host, int port) : redisClient(host, port) {}

void CLI::run() {
    if (!redisClient.connectToServer()) {
        return;
    }

    std::cout << "Connected to Redis at " << redisClient.getSocketFD() << "\n";
    std::string host = redisClient.getHost(); // "127.0.0.1"
    int port = redisClient.getPort();         // 6397

    while (true) {
        std::cout << host << ":" << port << "> ";
        std::cout.flush(); // cleaning the buffer
        std::string line;
        if (!std::getline(std::cin, line))
            break;

        line = trim(line);

        if (line.empty())
            continue;
        if (line == "quit") {
            std::cout << "Goodbye.\n";
            break;
        }
        if (line == "help") {
            std::cout << "Displaing help\n";
            continue;
        }

        // Split command into tokens
        std::vector<std::string> args = CommandHandler::splitArgs(line);
        if (args.empty())
            continue;

        for (const auto &arg : args) {
            std::cout << arg << "\n";
        }
    }
}
