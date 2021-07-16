#include "json.hpp" // https://github.com/nlohmann/json
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "GameTimeline.h"
#include "RealTimeline.h"
#include "Event.h"
#include "EventManager.h"
#include "Alien.h"
#include "Bullet.h"
#include "MoveByPattern.h"
#include "ScriptManager.h"
#include "dukglue/dukglue.h"
#include <iostream>
#include <zmq.hpp>
#include <thread>
#include <mutex>
#include "Thread.h"
#include "ServerEventHandler.h"

// for convenience
using json = nlohmann::json;

GameTimeline timeline(new RealTimeline(), 500);
std::vector<Alien*> aliens;
int aliens_per_row = 6;
int num_columns = 3;
bool paused = false;

void run_wrapper(Thread* fe)
{
    fe->run();
}


int main() {

    ScriptManager scripter = ScriptManager();
    dukglue_register_method(scripter.getContext(), &Alien::setPos, "setPos");
    dukglue_register_method(scripter.getContext(), &Alien::setX, "setX");
    dukglue_register_method(scripter.getContext(), &Alien::setY, "setY");
    dukglue_register_method(scripter.getContext(), &Alien::xPos, "xPos");
    dukglue_register_method(scripter.getContext(), &Alien::yPos, "yPos");
    dukglue_register_method(scripter.getContext(), &Alien::addToX, "addToX");
    dukglue_register_method(scripter.getContext(), &Alien::addToY, "addToY");
    dukglue_register_method(scripter.getContext(), &Alien::getMoveComponent, "getMoveComponent");
    dukglue_register_method(scripter.getContext(), &MoveByPattern::setMovementDirection, "setMovementDirection");
    scripter.loadScript("scriptFile.js");

    ServerEventHandler seh(1);
    EventManager::registerHandler("reset", &seh);

    // Create aliens
    for (int j = 0; j < num_columns; j++) {
        for (int i = 0; i < aliens_per_row; i++) {
            aliens.push_back(new Alien(1 + (60 * i), 1 + (30 * j), 50, 30, new MoveByPattern(1 + (60 * i), 420, &scripter), nullptr, nullptr, nullptr));
        }
    }

    // Set up context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    //  Wait for first request from client
    zmq::message_t request;
    socket.recv(&request);
    std::string requestString = std::string(static_cast<char*>(request.data()));
    json j = json::parse(requestString);
    int id = j["id"];

    if (id == -1) {
        std::cout << "Client connected" << std::endl;
        json j_reply;
        j_reply["numAliens"] = aliens_per_row * num_columns;
        //send reply
        std::string replyString = j_reply.dump();
        char replyCharArray[50];
        strcpy(replyCharArray, replyString.c_str());
        zmq::message_t reply(50);
        std::copy(replyCharArray, replyCharArray + 50, static_cast<char*>(reply.data()));
        socket.send(reply);
    }

    //create threads
    std::mutex m;
    std::condition_variable cv;
    Thread t1(0, &m, &cv);
    std::thread thread0(run_wrapper, &t1);

    while (true) {

        //  Wait for request from client
        zmq::message_t request;
        socket.recv(&request);
        std::string requestString = std::string(static_cast<char*>(request.data()));
        //std::cout << requestString << std::endl;
        json j = json::parse(requestString);
        int id = j["id"];

        // get any events in message and raise on server
        for (int i = 0; i < j["events"].size(); i++) {
            paused = true;
            Event e(j["events"][i]);
            {
                std::unique_lock<std::mutex> lock(m);
                EventManager::raiseAndHandleNow(e);
            }
        }

        // Send reply containing alien positions
        if (id != -1) {
            json j_reply;
            j_reply["id"] = 1;
            j_reply["aliens"] = json::array({});
            for (int i = 0; i < aliens.size(); i++) {
                {
                    std::unique_lock<std::mutex> lock(m);
                    j_reply["aliens"].emplace_back(aliens[i]->toJSON());
                }
            }
            //send reply
            std::string replyString = j_reply.dump();
            char replyCharArray[1000];
            strcpy(replyCharArray, replyString.c_str());
            zmq::message_t reply(1000);
            std::copy(replyCharArray, replyCharArray + 1000, static_cast<char*>(reply.data()));
            socket.send(reply);
        }

        paused = false;

    }
}