#pragma once
#include <atomic>


class SpinLock
{
public:
    SpinLock()
    {
         flag_.clear();
    }
    void lock()
    {
        while(flag_.test_and_set())
        {

        }

    }
    void unlock()
    {
        flag_.clear();
    }

private:
    std::atomic_flag flag_;
};
