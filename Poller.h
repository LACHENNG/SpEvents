/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-25 19:46:13
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-26 21:22:53
 * @FilePath: /spMuduo/Poller.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

/*
*  Description: Poller class is a wrapper for IO multiplexing, which is a indirect members of its owner 
* `EventLoop`. It shouldn`t be called in place other than its owner loop(which is an IO thread)
*  Poller doesn`t own Channel. So Channel should unregister itself when deconstructing
*/
#ifndef _POLLER_H_
#define _POLLER_H_

#include <vector> 
#include <unordered_map>
#include <boost/noncopyable.hpp>

class EventLoop;
class Channel;
struct pollfd;

// Find the active fd from the given fds
class Poller : public boost::noncopyable{
    typedef std::vector<Channel*> ChannelList;  
    typedef std::unordered_map<int, Channel*> ChannelMap;
public:

    Poller(EventLoop* owerLoopArg);
    ~Poller();
    // FIXME: change the return type to TimeStamp
    void poll(ChannelList& channels);
    void fillActiveChannels(int nEvents, ChannelList& activateChannels);
    void updateChannel(Channel* ch);
private:
    EventLoop* owner_loop_;
    std::vector<pollfd> pollfds_;
    ChannelMap channels_;
};


#endif /* _POLLER_H_ */