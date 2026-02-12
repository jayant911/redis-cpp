# my_redis_cli — Client

A Redis CLI client built from scratch in **C++17** using raw Berkeley sockets and the **RESP2** protocol. No external dependencies.

---

## 📦 Build & Run

```bash
# Build
make

# Run (default: 127.0.0.1:6379)
./bin/my_redis_cli

# Custom host & port
./bin/my_redis_cli -h 192.168.1.10 -p 6380

# Clean / Rebuild
make clean
make rebuild
```

> **Requires:** g++ with C++17, make, and a running Redis server.

---

## 💻 REPL Usage

```
127.0.0.1:6379> PING
PONG
127.0.0.1:6379> SET name "Jayanta"
OK
127.0.0.1:6379> GET name
Jayanta
127.0.0.1:6379> DEL name
1
127.0.0.1:6379> quit
Goodbye.
```

| Command | Action |
|---|---|
| `help` | Print help info |
| `quit` | Exit the REPL |

Supports **quoted strings** — e.g. `SET key "hello world"`.

---

## 🏗️ Architecture

```
client/
├── include/
│   ├── CLI.h
│   ├── CommandHandler.h
│   ├── RedisClient.h
│   └── ResponseParser.h
├── src/
│   ├── main.cpp
│   ├── CLI.cpp
│   ├── CommandHandler.cpp
│   ├── RedisClient.cpp
│   └── ResponseParser.cpp
└── Makefile
```

### Components

#### `RedisClient`
Manages the TCP socket lifecycle using Berkeley sockets and `getaddrinfo` (IPv4/IPv6).

| Method | Description |
|---|---|
| `connectToServer()` | Resolves the host and opens a TCP connection |
| `sendCommand()` | Sends raw bytes over the socket |
| `disconnect()` | Closes the socket |
| `getSocketFD()` | Returns the file descriptor for direct reads |

#### `CommandHandler`
Stateless utility that transforms user input into wire-format commands.

| Method | Description |
|---|---|
| `splitArgs()` | Tokenizes input with quoted-string support via regex |
| `buildRESPcommand()` | Serializes tokens into RESP2 format (`*N\r\n$L\r\narg\r\n...`) |

#### `ResponseParser`
Reads from the socket and deserializes RESP2 responses.

| RESP Type | Prefix | Example |
|---|---|---|
| Simple String | `+` | `+OK\r\n` |
| Error | `-` | `-ERR unknown command\r\n` |
| Integer | `:` | `:42\r\n` |
| Bulk String | `$` | `$5\r\nhello\r\n` |
| Array | `*` | `*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n` |

Handles `(nil)` for null bulk strings/arrays and recursively parses nested arrays.

#### `CLI`
The REPL controller — reads input, dispatches to `CommandHandler` → `RedisClient` → `ResponseParser`, and prints results.

---

## 🔧 RESP2 Protocol

Commands are sent as RESP arrays. For example, `SET mykey hello` becomes:

```
*3\r\n
$3\r\nSET\r\n
$5\r\nmykey\r\n
$5\r\nhello\r\n
```

Responses are parsed based on the first byte (`+`, `-`, `:`, `$`, `*`).

---

## 🗺️ TODO

- [ ] Wire up one-shot command mode (`./my_redis_cli PING`)
- [ ] Connect `printHelp()` to the `help` REPL command
- [ ] Implement `handleSubscription()` for Pub/Sub
- [ ] Add `readline` / `linenoise` for command history & arrow keys
- [ ] Tab completion for Redis commands
- [ ] AUTH / password support
- [ ] TLS/SSL encrypted connections
- [ ] Reconnection on server disconnect
- [ ] `--raw` / `--csv` output modes
- [ ] Pipeline & `MULTI`/`EXEC` transaction support
