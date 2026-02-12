# redis-cpp

A minimal Redis CLI client written in **C++17** from scratch — no third-party libraries, just raw Berkeley sockets and the [RESP (Redis Serialization Protocol)](https://redis.io/docs/reference/protocol-spec/).

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Linux-lightgrey?logo=linux)

---

## ✨ Features

- **Interactive REPL** – Connect and send Redis commands in real-time, just like `redis-cli`.
- **RESP Protocol** – Fully implements RESP2 serialization & deserialization (simple strings, errors, integers, bulk strings, arrays).
- **IPv4 / IPv6** – Automatically resolves and connects via either protocol using `getaddrinfo`.
- **Quoted arguments** – Supports quoted strings so you can `SET "hello world" 42`.
- **Configurable host & port** – Pass `-h` and `-p` flags or use the default `127.0.0.1:6379`.
- **Clean OOP design** – Separated into focused, single-responsibility classes.

---

## 🏗️ Architecture

```
client/
├── include/
│   ├── CLI.h              # REPL loop & user interaction
│   ├── CommandHandler.h   # Input tokenizing & RESP formatting
│   ├── RedisClient.h      # TCP socket connection management
│   └── ResponseParser.h   # RESP response deserialization
├── src/
│   ├── main.cpp           # Entry point & argument parsing
│   ├── CLI.cpp
│   ├── CommandHandler.cpp
│   ├── RedisClient.cpp
│   └── ResponseParser.cpp
└── Makefile
```

| Component | Responsibility |
|---|---|
| **RedisClient** | Opens a TCP socket, sends raw bytes, and manages the connection lifecycle. |
| **CommandHandler** | Splits user input into tokens (with quoted-string support) and builds RESP-formatted commands. |
| **ResponseParser** | Reads from the socket and parses RESP2 replies into human-readable strings. |
| **CLI** | Runs the interactive REPL loop — reads input, dispatches commands, and prints responses. |

---

## 🚀 Getting Started

### Prerequisites

- **g++** with C++17 support
- **make**
- A running **Redis** server (e.g. `redis-server`)

### Build

```bash
cd client
make
```

The binary is output to `client/bin/my_redis_cli`.

### Run

```bash
# Default: connects to 127.0.0.1:6379
./bin/my_redis_cli

# Custom host & port
./bin/my_redis_cli -h 192.168.1.10 -p 6380
```

### Clean

```bash
make clean     # Remove build artifacts
make rebuild   # Clean + rebuild
```

---

## 💻 Usage

Once connected you get an interactive prompt:

```
127.0.0.1:6379> PING
PONG
127.0.0.1:6379> SET mykey "Hello World"
OK
127.0.0.1:6379> GET mykey
Hello World
127.0.0.1:6379> quit
Goodbye.
```

### Built-in commands

| Command | Description |
|---|---|
| `help` | Display help information |
| `quit` | Exit the REPL |

---

## 📚 What This Project Demonstrates

- **Socket Programming** – Creating and managing TCP connections with Berkeley sockets in C++.
- **Protocol Implementation** – Serializing commands and deserializing responses per the RESP2 spec.
- **OOP Principles** – Separation of concerns, single-responsibility classes, clean header/source split.
- **CLI Development** – Building an interactive terminal tool with argument parsing.

---

## 🗺️ Roadmap

- [ ] One-shot command execution (`./my_redis_cli PING`)
- [ ] Pub/Sub support
- [ ] Command history & `readline` integration
- [ ] TLS/SSL connections
- [ ] Connection pooling & reconnection logic
- [ ] Pipeline & transaction support (`MULTI`/`EXEC`)

---

## 🤝 Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request.

1. Fork the repo
2. Create your branch (`git checkout -b feature/awesome`)
3. Commit your changes (`git commit -m 'Add awesome feature'`)
4. Push to the branch (`git push origin feature/awesome`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the [MIT License](LICENSE).
