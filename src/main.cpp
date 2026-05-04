#include <SFML/Graphics.hpp>
#include "discord.h" // <--- Change this from discord_rpc.h
#include <iostream>
#include <string>

// Update function for the new SDK
void updateActivity(discord::Core* core, int secondsLeft) {
    discord::Activity activity{};
    activity.SetDetails("Staring into the Abyss.");
    
    if (secondsLeft <= 0) {
        activity.SetState("Found the bottom.");
    } else {
        std::string timeStr = std::to_string(secondsLeft / 60) + "m remaining";
        activity.SetState(timeStr.c_str());
    }

    activity.GetAssets().SetLargeImage("abyss_logo");
    
    core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
        // Activity updated
    });
}

int main() {
    // Initialize Discord Core (Replace 1234567890 with your actual Client ID)
    discord::Core* core{};
    auto result = discord::Core::Create(1234567890, DiscordCreateFlags_Default, &core);
    
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Abyss.", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false);

    sf::Texture img;
    if (!img.loadFromFile("void.jpg")) return -1;
    sf::Sprite sprite(img);

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
            static int lastUpdate = -1;
            if (elapsed != lastUpdate) {
                updateActivity(core, remaining);
                lastUpdate = elapsed;
            }
            core->RunCallbacks();
        }

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    delete core;
    return 0;
}
