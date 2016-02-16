#pragma once

#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>

typedef std::chrono::duration<std::chrono::high_resolution_clock::rep, std::chrono::high_resolution_clock::period> EventWaiterDuration;

template<typename TMsg>
class EventWaiter
{
public:
    EventWaiter()
    { }
    ~EventWaiter()
    { 
        std::unique_lock<std::mutex> lock(this->waitersLock);

        for (Waiter* waiter : this->waiters)
        {
            waiter->dtor = true;
        }
    }

    void waitEvent(const TMsg& msg)
    {
         Waiter self;
         self.dtor = false;

        {
            std::unique_lock<std::mutex> lock(this->waitersLock);
            this->waiters.push_back(&self);
        }

        {
            std::unique_lock<std::mutex> lock(self.lock);
            do
            {
                self.cond.wait(lock);
                while (!self.dtor && !self.msgs.empty() && self.msgs.front() != msg)
                    self.msgs.pop();
            } while (!self.dtor && self.msgs.empty());
        }

        if (!self.dtor)
        {
            std::unique_lock<std::mutex> lock(this->waitersLock);
            this->waiters.remove(&self);
        }
    }

    bool waitEvent(const TMsg& msg, EventWaiterDuration rel_time)
    {
        Waiter self;
        self.dtor = false;

        {
            std::unique_lock<std::mutex> lock(this->waitersLock);
            this->waiters.push_back(&self);
        }

        {
            std::cv_status status;
            std::unique_lock<std::mutex> lock(self.lock);
            do
            {
                std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
                status = self.cond.wait_for(lock, rel_time);
                rel_time -= (std::chrono::high_resolution_clock::now() - start);
                while (!self.dtor && !self.msgs.empty() && self.msgs.front() != msg)
                    self.msgs.pop();
            } while (!self.dtor && self.msgs.empty() && status != std::cv_status::timeout);
        }

        if (!self.dtor)
        {
            std::unique_lock<std::mutex> lock(this->waitersLock);
            this->waiters.remove(&self);
        }
        return !self.msgs.empty();
    }

    void triggerEvent(const TMsg& msg)
    {
        std::unique_lock<std::mutex> lock(this->waitersLock);

        for (Waiter* waiter : this->waiters)
        {
            std::unique_lock<std::mutex> wLock(waiter->lock);

            waiter->msgs.push(msg);
            waiter->cond.notify_one();
        }
    }

private:

    struct Waiter
    {
        std::mutex lock;
        std::condition_variable cond;
        std::queue<TMsg> msgs;
        bool dtor;
    };

    std::mutex waitersLock;
    std::list<Waiter*> waiters;


    EventWaiter(const EventWaiter&) = delete;
    EventWaiter& operator=(const EventWaiter&) = delete;
    EventWaiter(EventWaiter&&) = delete;
    EventWaiter& operator=(EventWaiter&&) = delete;
};