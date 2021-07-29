#ifndef USERTIMER_H
#define USERTIMER_H

typedef void (*Fun)(void);
class UserTimerManager;

class UserTimer
{
public:
    enum TimerType { ONCE, CIRCLE };

    UserTimer(UserTimerManager& manager);
    ~UserTimer();
    void Start(Fun fun, unsigned interval, TimerType timeType = CIRCLE);
    void Stop();

private:
    void OnTimer(unsigned long long now);
private:
    friend class UserTimerManager;
    UserTimerManager& manager_;
    TimerType timerType_;
    Fun timerFun_;
    unsigned interval_;
    unsigned long long expires_;

    int heapIndex_;
};

#endif // TIMER_H
