#include "BlockingQueue.hpp"

#include <future>
#include <chrono>
#include <iostream>
#include <vector>
#include <memory>


static const int round = 4000;
static const int thread = 10;


template <typename T>
struct Object {
public:
    Object(T value)
      : value(value)
    {}

public:
    T value;
};


using Integer = Object<int>;


void Producer(std::shared_ptr<BlockingQueue<std::shared_ptr<Integer>>> queue)
{
    for (int i = 0 ; i < round ; ++i) {
        auto item = std::make_shared<Integer>(i);
        queue->add(std::move(item));
    }
}

void Consumer(std::shared_ptr<BlockingQueue<std::shared_ptr<Integer>>> queue)
{
    // Force blocking in the last round.
    for (int i = 0 ; i < round + 1; ++i) {
        try {
            auto& itemPeek = queue->peek();
        } catch (const BlockingQueueExcept& e) {
            std::cout << e.what() << std::endl;
            return;
        }
        try {
            auto itemPoll = queue->poll();
        } catch (const BlockingQueueExcept& e) {
            std::cout << e.what() << std::endl;
            return;
        }
    }
}

int main()
{
    auto queue = std::make_shared<BlockingQueue<std::shared_ptr<Integer>>>();

    std::vector<std::future<void>> vecWrite;
    std::vector<std::future<void>> vecRead;

    for (int i = 0 ; i < thread ; ++i) {
        auto write = std::async(std::launch::async, Producer, queue);
        auto read = std::async(std::launch::async, Consumer, queue);
        vecWrite.push_back(std::move(write));
        vecRead.push_back(std::move(read));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(6500));
    queue->quit();

    for (int i = 0 ; i < thread ; ++i) {
        auto& write = vecWrite.back();
        auto& read = vecRead.back();

        if (write.valid()) {
            write.get();
        }
        if (read.valid()) {
            read.get();
        }

        vecWrite.pop_back();
        vecRead.pop_back();
    }

    return 0;
}