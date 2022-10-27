#include "EventLoop.h"
#include <thread>

EventLoop *eventLoop = nullptr;

void threadFunc(){
    eventLoop->loop();
}

int main(){

    eventLoop = new EventLoop();
    std::thread td(threadFunc);
    
    // td.detach();
    td.join();

    return 0;
}
