#include <SFML/Graphics.hpp>
#include <discord_rpc.h>
#include <iostream>
#include <time.h>

void updatePresence(int secondsLeft) {
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    
    // When time is up, change the status
    if (secondsLeft <= 0) {
        discordPresence.state = "Found the bottom of the Abyss.";
        discordPresence.details = "you also wasted 20 minutes LEL";
    } else {
        std::string timeStr = std::to_string(secondsLeft / 60) + "m remaining";
        discordPresence.state = "Staring into the Abyss.";
        discordPresence.details = timeStr.c_str();
    }
    
    discordPresence.largeImageKey = "abyss_logo"; 
    Discord_UpdatePresence(&discordPresence);
}

int main() {
    // Discord Initialization
    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    Discord_Initialize("YOUR_CLIENT_ID", &handlers, 1, NULL);

    // Fullscreen window
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Abyss.", sf::Style::Fullscreen);
    window.setMouseCursorVisible(false); // Hide cursor for immersion

    sf::Texture img;
    if (!img.loadFromFile("assets/void.jpg")) return -1;
    sf::Sprite sprite(img);

    sf::Clock timer;
    const int limit = 1200; 

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.key.code == sf::Keyboard::Escape) window.close();
        }

        int elapsed = (int)timer.getElapsedTime().asSeconds();
        int remaining = limit - elapsed;

        static int lastUpdate = -1;
        if (elapsed != lastUpdate) {
            updatePresence(remaining);
            lastUpdate = elapsed;
        }

        Discord_RunCallbacks();

        window.clear(sf::Color::Black);
        window.draw(sprite);
        window.display();
    }

    Discord_Shutdown();
    return 0;
}
