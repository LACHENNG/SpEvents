/*
 * @Author: LLLa
 * @Date: 2022-10-25 20:00:11
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-27 18:39:02
 * @FilePath: /spMuduo/Poller.cpp
 * @Description: 
 */
#include "Poller.h"
#include "Channel.h"
#include "EventLoop.h"

#include <poll.h>
#include <assert.h>
#include <iostream>
#include <algorithm>

Poller::Poller(EventLoop* ownerLoop) : owner_loop_(ownerLoop)
{
}
Poller::~Poller()
{
}

void Poller::poll(ChannelList& channels){
    int numEvents = ::poll(pollfds_.data(), pollfds_.size(), -1);
    if(numEvents > 0){
       fillActiveChannels(numEvents, channels);
    }else if(numEvents == 0){
        std::cout << "Nothing Happend" << std::endl;
    }else{
        std::cerr << "Poller::poll(); SYSTEM ERROR" << std::endl;
    }
}

void Poller::fillActiveChannels(int nEvents, ChannelList& activateChannels)
{
    for(const auto & pollfd : pollfds_){
        if(nEvents-- <= 0) break; 
        if(pollfd.revents > 0){
            const auto channelIter = channels_.find(pollfd.fd);
            assert(channelIter != channels_.end());
            auto& channel = channelIter->second;
            assert(channel->fd() == pollfd.fd);
            channel->set_revents(pollfd.revents);
            activateChannels.push_back(channel);
        }
    }
}

void Poller::updateChannel(Channel* ch){
    // Poller::updateChannel can only be call in owner_loop(EventLoop)
    owner_loop_->assertInLoopThread();
    
    // insert new one 
    int index = ch->index();
    
    // update an existing one  
    if(index >= 0 && index < pollfds_.size()){
        assert(channels_.find(ch->fd()) != channels_.end());
        assert(channels_[ch->fd()] == ch);
        pollfds_[index].events = ch->events();
        pollfds_[index].revents = ch->revents();
    }
    // insert a new one 
    else 
    {
        assert(channels_.find(ch->fd()) == channels_.end());
        // create a new fd to vector ' pollfds_'
        struct pollfd pfd;
        // FIXME: Is this right ? 
        pfd.revents = 0;
        pfd.events = ch->events();
        if(pfd.events == 0)
            pfd.fd = -1;
        else pfd.fd = ch->fd();
        pollfds_.emplace_back(std::move(pfd));
        ch->set_index(pollfds_.size() - 1);
        // add fd->channel mapping Entry
        channels_[ch->fd()] = ch;
        
    }
}