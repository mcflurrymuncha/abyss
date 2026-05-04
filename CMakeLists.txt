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
FetchContent_MakeAvailable(sfml)

# --- 2. DISCORD GAME SDK ---
FetchContent_Declare(
    discord_game_sdk
    URL https://dl-game-sdk.discordapp.net/2.5.6/discord_game_sdk.zip
)
FetchContent_MakeAvailable(discord_game_sdk)

# --- 3. SOURCE DISCOVERY ---
# This looks into the extracted folder to find the actual C++ wrapper files
file(GLOB DISCORD_SDK_SOURCES "${discord_game_sdk_SOURCE_DIR}/cpp/*.cpp")
file(GLOB DISCORD_SDK_HEADERS "${discord_game_sdk_SOURCE_DIR}/cpp/*.h")

# --- 4. BUILD TARGET ---
# We include our main file and all the Discord SDK source files found
add_executable(Abyss 
    src/main.cpp 
    ${DISCORD_SDK_SOURCES}
)

set_target_properties(Abyss PROPERTIES OUTPUT_NAME "Abyss.")

# Tell the compiler where the Discord headers are
target_include_directories(Abyss PRIVATE ${discord_game_sdk_SOURCE_DIR}/cpp)

# Link everything
target_link_libraries(Abyss 
    sfml-graphics 
    sfml-window 
    sfml-system
    ${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll.lib
)

# --- 5. ASSETS & DLLs ---
# Copy the Discord DLL to the build folder so the game can run
file(COPY "${discord_game_sdk_SOURCE_DIR}/lib/x86_64/discord_game_sdk.dll" DESTINATION ${CMAKE_BINARY_DIR})

# Copy your image
if(EXISTS "${CMAKE_SOURCE_DIR}/assets/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/assets/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
elseif(EXISTS "${CMAKE_SOURCE_DIR}/void.jpg")
    file(COPY "${CMAKE_SOURCE_DIR}/void.jpg" DESTINATION ${CMAKE_BINARY_DIR})
endif()
