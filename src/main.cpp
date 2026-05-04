#include <SFML/Graphics.hpp>
#include "discord.h"
#include <iostream>
#include <string>
#include <windows.h> // Required for console window

// Function to update the Discord Rich Presence
void updateActivity(discord::Core* core, int secondsLeft) {
    discord::Activity activity{};
    activity.SetDetails("Staring into the Abyss.");
    
    if (secondsLeft <= 0) {
        activity.SetState("The Abyss stared back.");
    } else {
        std::string timeStr = std::to_string(secondsLeft / 60) + "m remaining";
        activity.SetState(timeStr.c_str());
    }

    // Must match the key you uploaded to Discord Dev Portal
    activity.GetAssets().SetLargeImage("abyss_logo");
    
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        if (result != discord::Result::Ok) {
            std::cerr << "Activity Update Failed! Error: " << (int)result << std::endl;
        }
    });
}

int main() {
    // --- 1. FORCE DEBUG CONSOLE ---
    // This ensures a console window appears even in a GUI app
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    std::cout << "--- Abyss. Debug Console ---" << std::endl;

    // --- 2. DISCORD INITIALIZATION ---
    discord::Core* core{};
    // !!! REPLACE 1234567890 WITH YOUR ACTUAL APPLICATION ID !!!
    auto result = discord::Core::Create(1234567890, DiscordCreateFlags_NoRequireDiscord, &core);

    if (result == discord::Result::Ok) {
        std::cout << "Discord Core: Initialized Successfully" << std::endl;
    } else {
        std::cout << "Discord Core: Failed (Error " << (int)result << ")" << std::endl;
        std::cout << "Check if Client ID is correct and Discord is running." << std::endl;
    }

    // --- 3. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Abyss.", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    // --- 4. TEXTURE & SPRITE ---
    sf::Texture img;
    if (!img.loadFromFile("void.jpg")) {
        std::cout << "CRITICAL: void.jpg NOT FOUND in the current directory!" << std::endl;
    } else {
        std::cout << "Image Loaded: void.jpg" << std::endl;
    }

    sf::Sprite sprite(img);
    
    // Scale image to fill screen
    sf::Vector2u imgSize = img.getSize();
    sf::Vector2u winSize = window.getSize();
    if (imgSize.x > 0 && imgSize.y > 0) {
        sprite.setScale(
            (float)winSize.x / imgSize.x,
            (float)winSize.y / imgSize.y
        );
    }

    // --- 5. TIMER & LOOP ---
    sf::Clock timer;
    const int limit = 1200; // 20 minutes

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || 
               (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        int elapsed = (int)timer.getElapsedTime().asSeconds();
        int remaining = limit - elapsed;

        if (core) {
            static int lastUpdateSec = -1;
            if (elapsed != lastUpdateSec) {
                updateActivity(core, remaining);
                std::cout << "Discord Update Sent (T-" << remaining << "s)" << std::endl;
                lastUpdateSec = elapsed;
            }
            // Run callbacks to keep the connection alive
            core->RunCallbacks();
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    std::cout << "Closing game..." << std::endl;
    delete core;
    return 0;
}
