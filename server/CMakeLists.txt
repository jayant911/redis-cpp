cmake_minimum_required(VERSION 3.21)

project(redis_cpp LANGUAGES CXX)

find_package(Threads REQUIRED)

# ---------- Common library ----------
# add_library(redis_common
#     src/common/protocol.cpp
# )
#
# target_include_directories(redis_common
#     PUBLIC src/common
# )
#
# target_compile_features(redis_common
#     PUBLIC cxx_std_20
# )

# ---------- Server executable ----------
add_executable(redis_server
    src/server/main.cpp
)

target_link_libraries(redis_server
    PRIVATE Threads::Threads
)

# ---------- Client executable ----------
add_executable(redis_client
    src/client/main.cpp
)

target_link_libraries(redis_client
    PRIVATE Threads::Threads
)

