/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-22 20:07:34
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-27 14:28:26
 * @FilePath: /spMuduo/Channel.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef _CHANNEL_H_
#define _CHANNEL_H_
#include <functional>
/* forward decleartion*/
class EventLoop;

class Channel{
public:
    using EventCallback = std::function<void (void)>;
    Channel(EventLoop* loop, int fd);
    void handleEvent();
    void setReadCallback(const EventCallback& cb);
    void setWriteCallback(const EventCallback& cb);
    void setErrorCallback(const EventCallback& cb);

    void enableReading();
    void enableWriting();

    int index();
    void set_index(int index);
    
    int fd();

    int events();
    void set_events(int events);
    int revents();
    void set_revents(int revents);
    
    void updateChannel();
 
private:

    static const int NONE_EVENT;
    static const int READ_EVENT;
    static const int WRITE_EVENT;

    EventLoop* owner_loop_;
    const int fd_;
    int events_;  // a bit mask specifying the events this Channel interested in for the decriptor fd.
    int revents_; // an output parameter, filled by the kernel with the event that actually occurred.
    int index_;   // used by Poller
    
    EventCallback read_cb_;
    EventCallback write_cb_;
    EventCallback error_cb_;
};


#endif /*_CHANNEL_H_*/