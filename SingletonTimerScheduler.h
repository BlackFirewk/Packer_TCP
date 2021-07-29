#ifndef SINGLETONTIMERSCHEDULER_H
#define SINGLETONTIMERSCHEDULER_H

class UserTimer;
class UserTimerManager;

class SingletonTimerScheduler
{
public:
    SingletonTimerScheduler();
    ~SingletonTimerScheduler();

    UserTimer* getTimer1() const;
    void setTimer1(UserTimer* value);

    UserTimer* getTimer2() const;
    void setTimer2(UserTimer* value);

    UserTimerManager* getTimerManager() const;
    void setTimerManager(UserTimerManager* value);

    static SingletonTimerScheduler& GetInstance()
    {
        static SingletonTimerScheduler instance;
        return instance;
    }

private:
    UserTimerManager* timerManager;
    UserTimer* timer1;
    UserTimer* timer2;

};

#endif // SINGLETONTIMERSCHEDULER_H
