#include "Thread.h"
#include "GameTimeline.h"
#include "Alien.h"
#include <vector>

extern GameTimeline timeline;
extern std::vector<Alien*> aliens;
extern bool paused;

Thread::Thread(int i, std::mutex* _mutex, std::condition_variable* _condition_variable) {
    this->i = i;
    this->_mutex = _mutex;
    this->_condition_variable = _condition_variable;
}

void Thread::run() {
    if (i == 0 && !paused) {
        try {
            float time_last_updated = timeline.getTime();
            while (true) {
                // Update aliens
                float time_passed = timeline.getTime() - time_last_updated;
                time_last_updated = timeline.getTime();
                for (int i = 0; i < time_passed; i++) {
                    for (int j = 0; j < aliens.size(); j++) {
                        aliens[j]->update();
                    }

                }
            }
        }
        catch (...) {
            std::cerr << "Thread " << i << " caught exception." << std::endl;
        }
    }
}

