cmake_minimum_required(VERSION 3.14)
project(Abyss VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- 1. SFML ---
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# --- 2. DISCORD GAME SDK ---
FetchContent_Declare(
    discord_game_sdk
    URL https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip
)
FetchContent_MakeAvailable(discord_game_sdk)

# --- 3. BUILD ---
add_executable(Abyss src/main.cpp)
set_target_properties(Abyss PROPERTIES OUTPUT_NAME "Abyss.")

# Point to the Game SDK headers and library
target_include_directories(Abyss PRIVATE ${discord_game_sdk_SOURCE_DIR}/cpp)

# Link SFML and the Discord Game SDK (Windows 64-bit path)
target_link_libraries(Abyss 
    sfml-graphics 
    sfml-window 
    sfml-system
    ${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll.lib
)

# --- 4. ASSETS ---
if(EXISTS "${CMAKE_SOURCE_DIR}/assets/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/assets/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
elseif(EXISTS "${CMAKE_SOURCE_DIR}/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
endif()

# Copy the Discord DLL to the build folder so it can be packaged
file(COPY ${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll DESTINATION ${CMAKE_BINARY_DIR})
