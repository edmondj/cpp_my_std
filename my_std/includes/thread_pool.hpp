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

#include <atomic>
#include <future>
#include <mutex>
#include <thread>
#include <vector>
#include <type_traits>
#include "cross_thread_queue.hpp"

namespace my_std
{
    template<typename TRet>
    class thread_pool
    {
    private:
        typedef std::packaged_task<TRet()> task;
        typedef cross_thread_queue<task> task_queue;
        typedef std::vector<std::thread> thread_array;

    public:
        typedef typename thread_array::size_type thread_size_type;
        typedef typename task_queue::size_type queue_size_type;
        typedef TRet result_type;

        thread_pool(thread_size_type nb_thread)
            : _ths(nb_thread), _pause(false), _idle_count(nb_thread)
        {
            for (thread_size_type i = 0; i < nb_thread; i++)
            {
                _ths[i] = std::thread(&thread_pool::_thread_loop, this);
            }
        }

        thread_pool(thread_size_type nb_thread, queue_size_type max_size)
            : thread_pool(nb_thread)
        {
            this->_queue.set_max_size(max_size);
        }

        ~thread_pool()
        {
            this->cancel_all(); // clear queue
            this->resume(); // unpause thread
            this->_queue.dispose(); // every thread should be blocked at pop() or working
            for (std::thread& th : this->_ths) // wait for all threads to stop
            {
                th.join();
            }
        }

        thread_size_type nb_thread() const
        {
            return this->_ths.size();
        }

        void set_task_queue_max_size(queue_size_type max_size)
        {
            this->_queue.set_max_size(max_size);
        }

        void unset_task_queue_max_size()
        {
            this->_queue.unset_max_size();
        }

        template<typename TFunc, typename... TArgs>
        std::future<result_type> add_task(TFunc&& func, TArgs&&... args)
        {
            static_assert(std::is_convertible<decltype(std::bind(std::move(func), args...)), std::function<TRet()>>::value, "Add taks must provide a function and all its argument");

            task t(std::bind(std::move(func), args...));
            std::future<result_type> sent = t.get_future();

            this->_queue.push(std::move(t));
            return sent;
        }

        // known issue: might return if a task was pushed between wait_empty and idle check.
        void wait_all()
        {
            this->_queue.wait_empty(); // wait for no tasks left

                                       // wait for all threads to finish their works
            {
                std::unique_lock<std::mutex> lock(this->_idle_lock);

                while (this->_idle_count != this->_ths.size())
                    this->_idle_condvar.wait(lock);
            }
        }

        template<class Clock, class Duration>
        std::cv_status wait_all_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
        {
            std::cv_status sent = this->_queue.wait_empty_until(timeout_time);

            std::unique_lock<std::mutex> lock(this->_idle_lock);

            while (sent == std::cv_status::no_timeout && this->_idle_count != this->_ths.size())
                sent = this->_idle_condvar.wait_until(timeout_time);
            return sent;
        }

        template<typename Rep, typename Period>
        std::cv_status wait_all_for(const std::chrono::duration<Rep, Period>& rel_time)
        {
            return this->_queue.wait_empty_until(std::chrono::steady_clock::now() + rel_time);
        }

        void cancel_all()
        {
            this->_queue.clear();
        }

        void pause()
        {
            std::unique_lock<std::mutex> lock(this->_pause_lock);

            this->_pause = true;
        }

        void resume()
        {
            std::unique_lock<std::mutex> lock(this->_pause_lock);

            this->_pause = false;
            this->_pause_condvar.notify_all();
        }

    private:
        void _thread_loop()
        {
            task t;
            while (this->_queue.pop(t)) // gets a new task, return false when thread_pool is destroyed.
            {
                // check pause
                {
                    std::unique_lock<std::mutex> lock(this->_pause_lock);

                    while (this->_pause)
                        this->_pause_condvar.wait(lock);
                }
                // mark thread as working
                {
                    std::unique_lock<std::mutex> lock(this->_idle_lock);
                    --this->_idle_count;
                }
                t(); // do work
                     // mark thread as idling
                {
                    std::unique_lock<std::mutex> lock(this->_idle_lock);
                    ++this->_idle_count;
                    this->_idle_condvar.notify_all();
                }
            }
        }

        task_queue _queue;
        thread_array _ths;

        std::mutex _pause_lock;
        bool _pause;
        std::condition_variable _pause_condvar;

        std::mutex _idle_lock;
        thread_size_type _idle_count;
        std::condition_variable _idle_condvar;

        thread_pool(const thread_pool&) = delete;
        thread_pool& operator=(const thread_pool&) = delete;
        thread_pool(thread_pool&&) = delete;
        thread_pool& operator=(thread_pool&&) = delete;
    };
}
