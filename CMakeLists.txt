cmake_minimum_required(VERSION 3.14)
project(Abyss VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

# --- SFML ---
FetchContent_Declare(sfml GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# --- Discord Game SDK ---
FetchContent_Declare(
    discord_game_sdk
    URL https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip
)
FetchContent_MakeAvailable(discord_game_sdk)

# --- Create Executable ---
# IMPORTANT: We MUST include the discord.cpp file from the SDK so the functions work
add_executable(Abyss 
    src/main.cpp 
    ${discord_game_sdk_SOURCE_DIR}/cpp/discord.cpp 
    ${discord_game_sdk_SOURCE_DIR}/cpp/core.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/types.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/activity_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/application_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/voice_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/user_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/achievement_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/image_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/lobby_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/network_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/overlay_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/relationship_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/storage_manager.cpp
    ${discord_game_sdk_SOURCE_DIR}/cpp/store_manager.cpp
)

set_target_properties(Abyss PROPERTIES OUTPUT_NAME "Abyss.")

# Tell the compiler where to find discord.h
target_include_directories(Abyss PRIVATE ${discord_game_sdk_SOURCE_DIR}/cpp)

# Link SFML and the SDK library
target_link_libraries(Abyss 
    sfml-graphics sfml-window sfml-system
    ${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll.lib
)

# Copy Assets and DLL to build folder
file(COPY "${CMAKE_SOURCE_DIR}/assets/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
file(COPY "${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll" DESTINATION ${CMAKE_BINARY_DIR})
