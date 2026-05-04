cmake_minimum_required(VERSION 3.14)
project(Abyss.)

# 1. Fetch SFML automatically
include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(sfml)

# 2. Fetch Discord RPC automatically
FetchContent_Declare(
    discord_rpc
    GIT_REPOSITORY https://github.com/discord/discord-rpc.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(discord_rpc)

# 3. Build the game
add_executable(Abyss. src/main.cpp)

# 4. Link everything
target_link_libraries(Abyss. sfml-graphics sfml-window sfml-system discord-rpc)

# 5. Copy assets to build folder
file(COPY assets/void.jpg DESTINATION ${CMAKE_BINARY_DIR})
