cmake_minimum_required(VERSION 3.14)

# Project name is safe for Windows filesystem
project(Abyss VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- 1. SFML SETUP ---
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
# Speed up build by skipping what we don't need
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# --- 2. DISCORD RPC SETUP ---
FetchContent_Declare(
    discord_rpc
    GIT_REPOSITORY https://github.com/discord/discord-rpc.git
    GIT_TAG v3.4.0
)

# CRITICAL FIX: Disable the targets that cause the clang-format error
set(ENABLE_CLANG_FORMAT OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(discord_rpc)

# --- 3. BUILD TARGET ---
# Ensure your code is at src/main.cpp
add_executable(Abyss src/main.cpp)

# Give the file the actual name with the dot
set_target_properties(Abyss PROPERTIES OUTPUT_NAME "Abyss.")

# Link the libraries
target_link_libraries(Abyss 
    sfml-graphics 
    sfml-window 
    sfml-system 
    discord-rpc
)

# --- 4. ASSET COPYING ---
# Copies void.jpg to the build folder so it's ready for the zip
if(EXISTS "${CMAKE_SOURCE_DIR}/assets/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/assets/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
elseif(EXISTS "${CMAKE_SOURCE_DIR}/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
endif()
