#include "json.hpp" // https://github.com/nlohmann/json
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "GameTimeline.h"
#include "RealTimeline.h"
#include "Event.h"
#include "EventManager.h"
#include "Player.h"
#include "Bullet.h"
#include "CollisionMediator.h"
#include "DrawAsRectangle.h"
#include "MoveByUserInput.h"
#include "ScriptManager.h"
#include "dukglue/dukglue.h"
#include "InputHandler.h"
#include "Alien.h"
#include <zmq.hpp>
#include <iostream>

// for convenience
using json = nlohmann::json;

std::vector<Alien*> aliens;

constexpr auto WIDTH = 800;
constexpr auto HEIGHT = 600;

GameTimeline timeline(new RealTimeline(), 10);
bool up_pressed = false;

int main() {
	float time_last_updated = timeline.getTime();
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Space Invaders", sf::Style::Default);

    // Create textures
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("space-invaders-player-sprite.png"))
        exit(0);
    sf::Texture alien_texture;
    if (!alien_texture.loadFromFile("space-invaders-alien-sprite.png"))
        exit(0);

    // Create and register event handlers
    InputHandler ih(0);
    EventManager::registerHandler("right arrow press", &ih);
    EventManager::registerHandler("left arrow press", &ih);
    EventManager::registerHandler("space press", &ih);
    EventManager::registerHandler("reset", &ih);

    // Set up socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    // Send request to server
    json j;
    j["id"] = -1;
    std::string requestString = j.dump();
    char replyCharArray[100];
    strcpy(replyCharArray, requestString.c_str());
    zmq::message_t request(100);
    std::copy(replyCharArray, replyCharArray + 100, static_cast<char*>(request.data()));
    std::cout << "Waiting for server" << std::endl;
    socket.send(request);

    //  Get the reply 
    zmq::message_t reply;
    socket.recv(&reply);
    std::string replyString = std::string(static_cast<char*>(reply.data()));
    json j_reply = json::parse(replyString);
    int num_aliens = j_reply["numAliens"];
    std::cout << "Connected to server" << std::endl;

    // Create player
    Bullet player_bullet(385, 560, 5, "kill alien", -1, new DrawAsRectangle(window, sf::Color(255, 255, 255)));
    CollisionMediator player_cm;
    Player player(385, 560, 30, 30, new MoveByUserInput(1), &player_cm, new DrawAsRectangle(window, player_texture, 415, 257), &player_bullet);

    // Create aliens
    CollisionMediator alien_cm;
    alien_cm.addGameObject(player_bullet);
    for (int i = 0; i < num_aliens; i++) {
        Bullet alien_bullet(0, 0, 0.2, "kill player", 1, new DrawAsRectangle(window, sf::Color(0, 255, 0)));
        player_cm.addGameObject(alien_bullet);
        Alien* a = new Alien(10, 10, 50, 30, nullptr, &alien_cm, new DrawAsRectangle(window, alien_texture, 1057, 769), &alien_bullet);
        aliens.push_back(a);
        player_cm.addGameObject(*a);
    }

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Move Right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            std::map<std::string, variant> args;
            variant var{ variant::Type::TYPE_PLAYERPTR };
            var._asPlayerPtr = &player;
            args["player"] = var;
            Event e("right arrow press", args, timeline.getTime(), timeline.getTime());
            EventManager::raise(e);
        }

        // Move Left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            std::map<std::string, variant> args;
            variant var(variant::Type::TYPE_PLAYERPTR);
            var._asPlayerPtr = &player;
            args["player"] = var;
            Event e("left arrow press", args, timeline.getTime(), timeline.getTime());
            EventManager::raise(e);
        }

        // Shoot
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            if (!up_pressed) {
                up_pressed = true;
                std::map<std::string, variant> args;
                variant var(variant::Type::TYPE_PLAYERPTR);
                var._asPlayerPtr = &player;
                args["player"] = var;
                Event e("space press", args, timeline.getTime(), timeline.getTime());
                EventManager::raise(e);
            }
        }
        else {
            up_pressed = false;
        }

        // Send request to server
        json j;
        j["id"] = 1;
        if(EventManager::getServerQueueSize() > 0)
            j["events"] = EventManager::getEventsForServer();
        std::string requestString = j.dump();
        char replyCharArray[1000];
        strcpy(replyCharArray, requestString.c_str());
        zmq::message_t request(1000);
        std::copy(replyCharArray, replyCharArray + 100, static_cast<char*>(request.data()));
        //std::cout << "Waiting for server" << std::endl;
        socket.send(request);

        //  Get the reply 
        zmq::message_t reply;
        socket.recv(&reply);
        std::string replyString = std::string(static_cast<char*>(reply.data()));
        json j_reply = json::parse(replyString);
        //std::cout << "Receive reply" << std::endl;

        // Update alien positions based on server reply
        json j_aliens = j_reply["aliens"];
        for (int i = 0; i < aliens.size(); i++) {
            json a = j_aliens[i]["alien"];
            aliens[i]->setPos(a["x"].get<float>(), a["y"].get<float>());
        }


        // Update and draw game objects
        float time_passed = timeline.getTime() - time_last_updated;
        time_last_updated = timeline.getTime();
        for (int i = 0; i < time_passed; i++) {
            // Handle events
            EventManager::handleEvents(timeline.getTime());

            window.clear(sf::Color::Black);
            player.update();
            for (int a = 0; a < aliens.size(); a++) {
                aliens[a]->update();
                //std::cout << aliens[a]->xPos() << " " << aliens[a]->yPos() << std::endl;
            }
            window.display();

        }

    }
}