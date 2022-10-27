/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-18 20:46:18
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-27 11:55:34
 * @FilePath: /spMuduo/EventLoop.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _EVENTLOOP_H_
#define _EVENTLOOP_H_
#include <thread>
#include <boost/noncopyable.hpp>
#include <atomic>
#include <vector>
#include <memory>

class Channel;
class Poller;

class EventLoop : boost::noncopyable {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    void assertInLoopThread();
    bool isInLoopThread() const{return std::this_thread::get_id() == threadId_;}

    void updateChannel(Channel* ch);
    static EventLoop* getLoopOfCurrentThread();
private:
    void abortNotInLoopThread();
    
    const std::thread::id threadId_;

    std::atomic_bool looping_; /* atomic */
    std::atomic_bool quit_; 
private:
    std::vector<Channel*> activeChannels_;
    std::unique_ptr<Poller> pPoller_;
};




#endif /*_EVENTLOOP_H_*/