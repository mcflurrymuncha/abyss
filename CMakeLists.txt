cmake_minimum_required(VERSION 3.14)

# We name the project 'Abyss' to avoid filesystem errors with the trailing dot
project(Abyss VERSION 1.0)

# Ensure C++17 for SFML and modern coding standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. Fetch SFML (Graphics, Window, System)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
)
# We disable SFML examples and documentation to speed up the GitHub build
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(sfml)

# 2. Fetch Discord RPC (Legacy version)
FetchContent_Declare(
    discord_rpc
    GIT_REPOSITORY https://github.com/discord/discord-rpc.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(discord_rpc)

# 3. Define the Executable
# Make sure your main file is located at src/main.cpp
add_executable(Abyss src/main.cpp)

# 4. Set the Output Name to "Abyss."
# This is where we add the dot back for the final .exe filename
set_target_properties(Abyss PROPERTIES OUTPUT_NAME "Abyss.")

# 5. Include Directories
# FetchContent usually handles this, but we'll be explicit for Discord
target_include_directories(Abyss PRIVATE 
    ${discord_rpc_SOURCE_DIR}/include
)

# 6. Linking
# We link the SFML components and the Discord RPC library
target_link_libraries(Abyss 
    sfml-graphics 
    sfml-window 
    sfml-system 
    discord-rpc
)

# 7. Asset Management
# This copies your image into the build folder so the game can find it during testing
if(EXISTS "${CMAKE_SOURCE_DIR}/assets/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/assets/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
elseif(EXISTS "${CMAKE_SOURCE_DIR}/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
endif()
