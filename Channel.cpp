/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-22 20:17:59
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-27 15:13:29
 * @FilePath: /spMuduo/Channel.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <iostream>
#include <poll.h>
#include "Channel.h"
#include "EventLoop.h"

const int Channel::NONE_EVENT = 0;
const int Channel::WRITE_EVENT = POLLOUT;
const int Channel::READ_EVENT = POLLIN | POLLPRI;

Channel::Channel(EventLoop* loop, int fd) 
    : owner_loop_(loop), 
      fd_(fd),
      events_(Channel::NONE_EVENT),
      revents_(Channel::NONE_EVENT),
      index_(-1)
{ 
}

void Channel::handleEvent()
{
    if(revents_ & POLLNVAL){
        std::cerr << "Channel::handleEvent() POLLNVAL" << std::endl; 
    }

    if(revents_ & (POLLNVAL | POLLERR)){
        if(error_cb_) error_cb_();
    }
    
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)){
        if(read_cb_) read_cb_();
    }
    
    if(revents_ & POLLOUT){
        if(write_cb_) write_cb_();
    }
}

void Channel::setReadCallback(const EventCallback& read_cb)
{
    read_cb_ = read_cb;
}

void Channel::setWriteCallback(const EventCallback& write_cb)
{
    write_cb_ = write_cb;
}

void Channel::setErrorCallback(const EventCallback& error_cb)
{
    error_cb_ = error_cb;
}

void Channel::set_revents(int revents)
{
    revents_ = revents;
}

void Channel::set_events(int events){
    events_ = events;
}
void Channel::set_index(int indexArg){
    index_ = indexArg;
}
int Channel::index(){
    return index_;
}

int Channel::fd()
{
    return fd_;
}

int Channel::events(){
    return events_;
}

int Channel::revents(){
    return revents_;
}

void Channel::updateChannel(){
    owner_loop_->updateChannel(this);
}


void Channel::enableReading(){
    events_ = READ_EVENT;
}

void Channel::enableWriting(){
    events_ = WRITE_EVENT;
}


