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

void printHelp() {
    std::cout << "my_redis_cli 1.0.0\n"
              << "Usage: \n"
              << "      With arguments:            ./my_redis_cli -h <host> -p "
                 "<port>\n"
              << "      Default Host (127.0.0.1):  ./my_redis_cli -p <port>\n"
              << "      Default Port (6379):       ./my_redis_cli -h <host>\n"
              << "      One-shot execution:        ./my_redis_cli <command> "
                 "[arguments]\n"
              << "\n"
              << "Interactive Mode (REPL):\n"
              << "      ./my_redis_cli\n"
              << "      Type Redis commands directly.\n"
              << "\n"
              << "To get help about Redis commands type:\n"
              << "      \"help\" to display this help message\n"
              << "      \"quit\" to exit\n"
              << "\n"
              << "Examples:\n"
              << "      ./my_redis_cli PING\n"
              << "      ./my_redis_cli SET mykey \"Hello World\"\n"
              << "      ./my_redis_cli GET mykey\n"
              << "\n"
              << "To set my_redis_cli preferences:\n"
              << "      \":set hints\" enable online hints\n"
              << "      \":set nohints\" disable online hints\n"
              << "Set your preferences in ~/.myredisclirc\n"
              << std::endl;
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

        std::string command = CommandHandler::buildRESPcommand(args);
        if (!redisClient.sendCommand(command)) {
            std::cerr << "(Error) Failed to send command.\n";
            break;
        }

        // Parse and print response
        std::string response =
            ResponseParser::parseResponse(redisClient.getSocketFD());
        std::cout << response << "\n";
    }
    redisClient.disconnect();
}

void CLI::executeCommand(const std::vector<std::string> &args) {
    if (args.empty())
        return;

    std::string command = CommandHandler::buildRESPcommand(args);
    if (!redisClient.sendCommand(command)) {
        std::cerr << "(Error) Failed to send command.\n";
        return;
    }

    // parse and print response
    try {
        std::string response =
            ResponseParser::parseResponse(redisClient.getSocketFD());
        std::cout << response << "\n";
    } catch (const std::exception &e) {
        std::cerr << "(Error) Failed to parse response: " << e.what() << "\n";
        std::cerr << "Redis server might have disconnected.\n";
    } catch (...) {
        std::cerr << "(Error) Unknown error during response parsing.\n";
    }
}
