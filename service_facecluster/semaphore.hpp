#ifndef _SERVICE_SEMAPHORE_H_
#define _SERVICE_SEMAPHORE_H_

#include <condition_variable>
#include <mutex>

using namespace std;
// A simple implemetation of semaphore based on boost thread lib.
class Semaphore {
public:
    explicit Semaphore(int count) : count_(count), mutex_(), condition_() {}

    void Signal() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            ++count_;
        }
        condition_.notify_one();
    }

    void Wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0) {
            condition_.wait(lock);
        }
        --count_;
    }

private:
    int count_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

#endif  /*_SERVICE_SEMAPHORE_H_*/
