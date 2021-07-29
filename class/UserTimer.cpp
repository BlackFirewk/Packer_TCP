#include "UserTimer.h"
#include "UserTimerManager.h"


UserTimer::UserTimer(UserTimerManager& manager)
    : manager_(manager)
    , heapIndex_(-1)
{
    //todo
}

UserTimer::~UserTimer()
{
    Stop();
}

//inline void UserTimer::Start(Fun fun, unsigned interval, TimerType timeType)
void UserTimer::Start(Fun fun, unsigned interval, TimerType timeType)
{
    Stop();
    interval_ = interval;
    timerFun_ = fun;
    timerType_ = timeType;
    this->expires_ = this->interval_ + UserTimerManager::GetCurrentMillisecs();
    manager_.AddTimer(this);
}

void UserTimer::Stop()
{
    if (heapIndex_ != -1) {
        manager_.RemoveTimer(this);
        heapIndex_ = -1;
    }
}

void UserTimer::OnTimer(unsigned long long now)
{
    if (timerType_ == UserTimer::CIRCLE) {
        expires_ = interval_ + now;
        manager_.AddTimer(this);
    } else {
        heapIndex_ = -1;
    }
    timerFun_();
}
