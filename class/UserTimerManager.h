#ifndef USERTIMERMANAGER_H
#define USERTIMERMANAGER_H
#include <vector>
class UserTimer;

class UserTimerManager
{
public:
    static unsigned long long GetCurrentMillisecs();
    void DetectTimers();

private:
    friend class UserTimer;
    void AddTimer(UserTimer* timer);
    void RemoveTimer(UserTimer* timer);

    void UpHeap(int index);
    void DownHeap(int index);
    void SwapHeap(int, int index2);

private:
    struct HeapEntry {
        unsigned long long time;
        UserTimer* timer;
    };
    std::vector<HeapEntry> heap_;
};

#endif // TIMERMANAGER_H
