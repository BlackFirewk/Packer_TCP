#include "UserTimerManager.h"
#include "UserTimer.h"
#include <sys/time.h>

//debug
#include "SingletonUserLogger.h"


void UserTimerManager::AddTimer(UserTimer* timer)
{
    timer->heapIndex_ = heap_.size();
    HeapEntry entry = { timer->expires_, timer };
    heap_.push_back(entry);
    UpHeap(heap_.size() - 1);
}

void UserTimerManager::RemoveTimer(UserTimer* timer)
{
    int index = timer->heapIndex_;
    if (!heap_.empty() && index < heap_.size()) {
        if (index == heap_.size() - 1) {
            heap_.pop_back();
        } else {
            SwapHeap(index, heap_.size() - 1);
            heap_.pop_back();
            int parent = (index - 1) / 2;
            if (index > 0 && heap_[index].time < heap_[parent].time) {
                UpHeap(index);
            } else {
                DownHeap(index);
            }
        }
    }
}

void UserTimerManager::DetectTimers()
{
    unsigned long long now = GetCurrentMillisecs();

    while (!heap_.empty() && heap_[0].time <= now) {
        UserTimer* timer = heap_[0].timer;
        RemoveTimer(timer);
        timer->OnTimer(now);
//        LOG4CXX_INFO(SingletonUserLogger::GetInstance().getLogger(), "The timer has arrived");
    }
}

void UserTimerManager::UpHeap(int index)
{
    int parent = index >> 1;
    while (index > 0 && heap_[index].time < heap_[parent].time) {
        SwapHeap(index, parent);
        index = parent;
        parent = index >> 1;
    }
}

void UserTimerManager::DownHeap(int index)
{
    int child = (index << 1) + 1;
    while (child < heap_.size()) {
        int minChild = (child + 1 == heap_.size() || heap_[child].time < heap_[child + 1].time) ? child : child + 1;
        if (heap_[index].time < heap_[minChild].time) {
            break;
        }
        SwapHeap(index, minChild);
        index = minChild;
        child = (index << 1) + 1;
    }
}

void UserTimerManager::SwapHeap(int index1, int index2)
{
    HeapEntry tmp = heap_[index1];
    heap_[index1] = heap_[index2];
    heap_[index2] = tmp;
    heap_[index1].timer->heapIndex_ = index1;
    heap_[index2].timer->heapIndex_ = index2;
}

unsigned long long UserTimerManager::GetCurrentMillisecs()
{
    timeval tv;
    ::gettimeofday(&tv, 0);
    unsigned long long ret = tv.tv_sec;
    return ret * 1000 + tv.tv_usec / 1000;
}
