/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-18 20:57:19
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-27 13:25:26
 * @FilePath: /spMuduo/Eventloop.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#include <iostream>
#include <assert.h>
#include "EventLoop.h"
#include <poll.h>
#include "Poller.h"
#include "Channel.h"
#include <memory>
using namespace std;

thread_local EventLoop* t_loopInThisThread = NULL;

EventLoop::EventLoop() : threadId_(std::this_thread::get_id()),
                        pPoller_(new Poller(this)){
    t_loopInThisThread = this;
    
}

EventLoop::~EventLoop(){
    assert(!looping_.load());
    t_loopInThisThread = NULL;
}

void EventLoop::assertInLoopThread(){
    if(threadId_ != std::this_thread::get_id()){
        abortNotInLoopThread();
    }
}

void EventLoop::abortNotInLoopThread(){
    looping_ = false;
    cout << "aboutNotInLoopThrad()" << endl;
    exit(1);    
}

void EventLoop::loop(){
    assertInLoopThread();
    assert(!looping_);
    looping_.store(true);
    // looping_ = true; 

    // ::poll(NULL, 0, 5 * 1000);
    while(!quit_){
        activeChannels_.clear();
        pPoller_->poll(activeChannels_);
        for(auto& channel : activeChannels_){
            channel->handleEvent();
        }
    }
    looping_ = false;
    std::cout << "EventLoop " << this << " stop looping" << endl; 
}

void EventLoop::quit(){
    quit_ = true;
}

EventLoop* EventLoop::getLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::updateChannel(Channel* ch)
{
    pPoller_->updateChannel(ch);
}