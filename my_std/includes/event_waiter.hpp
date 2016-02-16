/*
Copyright (c) 2016 Julien Edmond

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <list>

namespace my_std
{
    template<typename TMsg>
    class event_waiter
    {
    public:
        event_waiter()
        { }

        ~event_waiter()
        {
            std::unique_lock<std::mutex> lock(this->waiters_lock);

            for (waiter* waiter : this->waiters)
            {
                waiter->dtor = true;
                waiter->cond.notify_all();
            }
        }

        void wait_event(const TMsg& msg)
        {
            waiter self;
            self.dtor = false;

            {
                std::unique_lock<std::mutex> lock(this->waiters_lock);
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
                std::unique_lock<std::mutex> lock(this->waiters_lock);
                this->waiters.remove(&self);
            }
        }

        template<typename TDuration>
        bool wait_event(const TMsg& msg, TDuration rel_time)
        {
            waiter self;
            self.dtor = false;

            {
                std::unique_lock<std::mutex> lock(this->waiters_lock);
                this->waiters.push_back(&self);
            }

            {
                std::cv_status status;
                std::unique_lock<std::mutex> lock(self.lock);
                status = self.cond.wait_for(lock, rel_time,
                    [msg, self]()
                {
                    while (!self.dtor && !self.msgs.empty() && self.msgs.front() != msg)
                        self.msgs.pop();
                    return self.dtor || !self.msgs.empty();
                });
            }

            if (!self.dtor)
            {
                std::unique_lock<std::mutex> lock(this->waiters_lock);
                this->waiters.remove(&self);
            }
            return !self.msgs.empty() && self.msgs.front() == msg;
        }

        void trigger_event(const TMsg& msg)
        {
            std::unique_lock<std::mutex> lock(this->waiters_lock);

            for (waiter* waiter : this->waiters)
            {
                std::unique_lock<std::mutex> wLock(waiter->lock);

                waiter->msgs.push(msg);
                waiter->cond.notify_one();
            }
        }

    private:

        struct waiter
        {
            std::mutex lock;
            std::condition_variable cond;
            std::queue<TMsg> msgs;
            bool dtor;
        };

        std::mutex waiters_lock;
        std::list<waiter*> waiters;
        
        event_waiter(const event_waiter&) = delete;
        event_waiter& operator=(const event_waiter&) = delete;
        event_waiter(event_waiter&&) = delete;
        event_waiter& operator=(event_waiter&&) = delete;
    };
}