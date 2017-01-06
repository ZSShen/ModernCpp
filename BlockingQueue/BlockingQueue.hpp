#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include <stdexcept>
#include <string>


const std::string BLOCKING_QUEUE_FORCE_QUIT = "Force Quit";


class BlockingQueueExcept : public std::runtime_error {
public:
    explicit BlockingQueueExcept(const std::string& msg)
      : std::runtime_error(msg)
    {}
    explicit BlockingQueueExcept(const char* msg)
      : std::runtime_error(msg)
    {}
};


template <typename T>
class BlockingQueue {
public:
    BlockingQueue()
      : shouldWait_(true)
    {}

    ~BlockingQueue() = default;
    BlockingQueue(const BlockingQueue&) = delete;
    BlockingQueue& operator=(const BlockingQueue&) = delete;
    BlockingQueue(BlockingQueue&&) = delete;
    BlockingQueue& operator=(BlockingQueue&&) = delete;


    void add(T&& item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(std::move(item));
        lock.unlock();
        cond_.notify_one();
    }

    void add(const T& item)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(item);
        lock.unlock();
        cond_.notify_one();
    }

    void remove()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (queue_.size() > 0) {
            queue_.pop();
        }
    }

    T& peek()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() {
            if (!shouldWait_) {
                throw BlockingQueueExcept(BLOCKING_QUEUE_FORCE_QUIT);
            }
            return queue_.size() > 0;
        });

        return queue_.front();
    }

    T poll()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]() {
            if (!shouldWait_) {
                throw BlockingQueueExcept(BLOCKING_QUEUE_FORCE_QUIT);
            }
            return queue_.size() > 0;
        });

        auto item = std::move(queue_.front());
        queue_.pop();
        return std::move(item);
    }

    void quit()
    {
        shouldWait_ = false;
        cond_.notify_all();
    }

private:
    std::atomic<bool> shouldWait_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<T> queue_;
};
