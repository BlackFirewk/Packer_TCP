#include "SingletonTimerScheduler.h"
#include "UserTimer.h"
#include "UserTimerManager.h"


SingletonTimerScheduler::SingletonTimerScheduler()
{
    timerManager = new UserTimerManager;
    timer1 = new UserTimer(*timerManager);
    timer2 = new UserTimer(*timerManager);
}

SingletonTimerScheduler::~SingletonTimerScheduler()
{
    delete timerManager;
    delete timer1;
    delete timer2;
}

UserTimer* SingletonTimerScheduler::getTimer1() const
{
    return timer1;
}

void SingletonTimerScheduler::setTimer1(UserTimer* value)
{
    timer1 = value;
}

UserTimer* SingletonTimerScheduler::getTimer2() const
{
    return timer2;
}

void SingletonTimerScheduler::setTimer2(UserTimer* value)
{
    timer2 = value;
}

UserTimerManager* SingletonTimerScheduler::getTimerManager() const
{
    return timerManager;
}

void SingletonTimerScheduler::setTimerManager(UserTimerManager* value)
{
    timerManager = value;
}
