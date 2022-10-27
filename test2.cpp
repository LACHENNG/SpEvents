/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2022-10-26 20:52:39
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2022-10-27 15:14:24
 * @FilePath: /spMuduo/test2.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <sys/timerfd.h>
#include <iostream>
#include "EventLoop.h"
#include "Channel.h"
#include "memory.h"


// void timeoutCb(){
   
// }

EventLoop loop;
void timeoutCb(){
    std::cout << "time out callback" << std::endl;
    loop.quit();
};

int main()
{
    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    
    struct itimerspec howlong;
    memset(&howlong, 0, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    Channel ch(&loop, timerfd);
    ch.enableReading();
    ch.setReadCallback(timeoutCb);
    ch.updateChannel();

    loop.loop(); 
    return 0;
}