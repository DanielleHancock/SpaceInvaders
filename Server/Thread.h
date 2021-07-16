#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <zmq.hpp>
#include "json.hpp"

class Thread {

private:
    int i;
    std::mutex* _mutex;
    std::condition_variable* _condition_variable;

public:
    Thread(int i, std::mutex* _mutex, std::condition_variable* _condition_variable);
    void run();
};