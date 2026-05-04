#include <SFML/Graphics.hpp>
#include "discord.h"
#include <iostream>
#include <string>
#include <vector>

// Function to update the Discord Rich Presence
void updateActivity(discord::Core* core, int secondsLeft) {
    discord::Activity activity{};
    activity.SetDetails("Staring into the Abyss.");
    
    if (secondsLeft <= 0) {
        activity.SetState("Abyss.");
    } else {
        // Simple countdown display: e.g., "15m remaining"
        std::string timeStr = std::to_string(secondsLeft / 60) + "m remaining";
        activity.SetState(timeStr.c_str());
    }

    // Ensure you have an asset named 'abyss_logo' in your Discord Dev Portal
    activity.GetAssets().SetLargeImage("abyss_logo");
    
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        // Update callback (optional logging)
    });
}

int main() {
    // --- 1. DISCORD INITIALIZATION ---
    discord::Core* core{};
    // REPLACE '1234567890' with your actual Discord Application ID
    auto result = discord::Core::Create(1234567890, DiscordCreateFlags_NoRequireDiscord, &core);

    // --- 2. WINDOW SETUP ---
    // Fullscreen mode for maximum immersion
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Abyss.", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);
    window.setFramerateLimit(60);

    // --- 3. TEXTURE & SPRITE ---
    sf::Texture img;
    if (!img.loadFromFile("void.jpg")) {
        // If image fails, the window stays open but stays black/gray
        std::cerr << "Failed to load void.jpg" << std::endl;
    }

    sf::Sprite sprite(img);
    
    // Scaling: This forces the image to stretch to your monitor's resolution
    sf::Vector2u imgSize = img.getSize();
    sf::Vector2u winSize = window.getSize();
    if (imgSize.x > 0 && imgSize.y > 0) {
        sprite.setScale(
            (float)winSize.x / imgSize.x,
            (float)winSize.y / imgSize.y
        );
    }

    // --- 4. TIMER & LOOP ---
    sf::Clock timer;
    const int limit = 1200; // 20 minutes

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // ESC key to exit
            if (event.type == sf::Event::Closed || 
               (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                window.close();
            }
        }

        // Logic for Discord Updates
        int elapsed = (int)timer.getElapsedTime().asSeconds();
        int remaining = limit - elapsed;

        if (core) {
            static int lastUpdateSec = -1;
            // Only update Discord once per second to avoid spamming the API
            if (elapsed != lastUpdateSec) {
                updateActivity(core, remaining);
                lastUpdateSec = elapsed;
            }
            core->RunCallbacks();
        }

        // --- 5. RENDERING ---
        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    // Cleanup
    delete core;
    return 0;
}
