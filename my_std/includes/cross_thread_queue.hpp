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

#include <queue>
#include <mutex>
#include <condition_variable>
#include <stdexcept>

namespace my_std
{
    ///
    /// Represents a cross thread queue.
    ///
    template<typename T>
    class cross_thread_queue
    {
    private:

        // keeps tracks of threads using the queue, useful during disposing.
        class _step_in
        {
        public:
            _step_in(cross_thread_queue* queue)
                : _queue(queue)
            {
                std::unique_lock<std::mutex> lock(this->_queue->_in_lock);
                ++this->_queue->_in_count;
            }

            ~_step_in()
            {
                std::unique_lock<std::mutex> lock(this->_queue->_in_lock);
                --this->_queue->_in_count;
                this->_queue->_in_cond_var.notify_all();
            }

        private:
            cross_thread_queue* _queue;
        };

    public:
        typedef typename std::queue<T>::value_type value_type;
        typedef typename std::queue<T>::size_type size_type;
        typedef typename std::queue<T>::reference reference;
        typedef typename std::queue<T>::const_reference const_reference;

        cross_thread_queue()
            : _disposed(false), _sized(false), _max_size(0), _in_count(0)
        { }

        cross_thread_queue(size_type max_size)
            : cross_thread_queue()
        {
            this->set_max_size(max_size);
        }

        ~cross_thread_queue()
        {
            this->dispose();
        }

        // once disposed, every pop will return false and all other action will throw.
        void dispose()
        {
            // 
            {
                std::unique_lock<std::mutex> lock(this->_queue_lock);

                this->_disposed = true;
                this->_new_element.notify_all();
                this->_element_popped.notify_all();
                this->_empty.notify_all();
            }

            // wait for every thread to step out of the queue
            {
                std::unique_lock<std::mutex> lock(this->_in_lock);

                while (this->_in_count > 0)
                    this->_in_cond_var.wait(lock);
            }
        }

        bool empty()
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            this->_assert_dispose();

            return this->_queue.empty();
        }

        void clear()
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            this->_assert_dispose();

            std::queue<T>().swap(this->_queue);
            this->_element_popped.notify_all();
            this->_empty.notify_all();
        }

        void wait_empty()
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            while (!this->_disposed && !this->_queue.empty())
                this->_empty.wait(lock);
            this->_assert_dispose();
        }

        template<class Clock, class Duration>
        std::cv_status wait_empty_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            std::cv_status sent = std::cv_status::no_timeout;

            if (!this->_disposed && !this->_queue.empty())
                sent = this->_empty.wait_until(lock, timeout_time);
            this->_assert_dispose();
            return sent;
        }

        template<typename Rep, typename Period>
        std::cv_status wait_empty_for(const std::chrono::duration<Rep, Period>& rel_time)
        {
            return this->wait_empty_until(std::chrono::steady_clock::now() + rel_time());
        }

        size_type size()
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            this->_assert_dispose();

            return this->_queue.size();
        }

        void set_max_size(size_type maxSize)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            this->_assert_dispose();

            this->_sized = true;
            this->_max_size = maxSize;
        }

        void unset_max_size()
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            this->_assert_dispose();

            this->_sized = false;
        }

        void push(const value_type& value)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            while (!this->_disposed && this->_sized && this->_queue.size() >= this->_max_size)
                this->_element_popped.wait(lock);
            this->_assert_dispose();
            this->_queue.push(value);
            this->_new_element.notify_all();
        }

        template<class Clock, class Duration>
        std::cv_status try_push_until(const value_type& value, const std::chrono::time_point<Clock, Duration>& timeout_time)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            std::cv_status sent = std::cv_status::no_timeout;

            if (!this->_disposed && this->_sized && this->_queue.size() >= this->_max_size)
                sent = this->_element_popped.wait_until(lock, timeout_time);
            this->_assert_dispose();
            if (sent == std::cv_status::no_timeout)
            {
                this->_queue.push(value);
                this->_new_element.notify_all();
            }
            return sent;
        }

        template<typename Rep, typename Period>
        std::cv_status try_push_for(const value_type& value, const std::chrono::duration<Rep, Period>& rel_time)
        {
            return this->try_push_until(value, std::chrono::steady_clock::now() + rel_time);
        }

        void push(value_type&& value)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            while (!this->_disposed && this->_sized && this->_queue.size() >= this->_max_size)
                this->_element_popped.wait(lock);
            this->_assert_dispose();
            this->_queue.push(std::move(value));
            this->_new_element.notify_one();
        }

        template<class Clock, class Duration>
        std::cv_status try_push_until(value_type&& value, const std::chrono::time_point<Clock, Duration>& timeout_time)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);
            std::cv_status sent = std::cv_status::no_timeout;

            if (!this->_disposed && this->_sized && this->_queue.size() >= this->_max_size)
                sent = this->_element_popped.wait_until(lock, timeout_time);
            this->_assert_dispose();
            if (sent == std::cv_status::no_timeout)
            {
                this->_queue.push(std::move(value));
                this->_new_element.notify_one();
            }
            return sent;
        }

        template<typename Rep, typename Period>
        std::cv_status try_push_for(value_type&& value, const std::chrono::duration<Rep, Period>& rel_time)
        {
            return this->try_push_until(std::move(value), std::chrono::steady_clock::now() + rel_time);
        }

        bool pop(value_type& output)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            while (!this->_disposed && this->_queue.empty())
                this->_new_element.wait(lock);
            if (this->_disposed || this->_queue.empty())
                return false;
            output = std::move(this->_queue.front());
            this->_queue.pop();
            this->_element_popped.notify_one();
            if (this->_queue.empty())
                this->_empty.notify_all();
            return true;
        }

        template<class Clock, class Duration>
        bool try_pop_until(value_type& output, const std::chrono::time_point<Clock, Duration>& timeout_time)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            if (!this->_disposed && this->_queue.empty())
                this->_new_element.wait_until(lock, timeout_time);
            if (this->_disposed || this->_queue.empty())
                return false;
            output = std::move(this->_queue.front());
            this->_queue.pop();
            this->_element_popped.notify_one();
            if (this->_queue.empty())
                this->_empty.notify_all();
            return true;
        }

        template<typename Rep, typename Period>
        bool try_pop_for(value_type& output, const std::chrono::duration<Rep, Period>& rel_time)
        {
            return this->try_pop_until(output, std::chrono::steady_clock::now() + rel_time);
        }

        bool try_front(T& out)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            if (this->_queue.empty())
                return false;
            out = this->_queue.front();
            return true;
        }

        bool try_back(T& out)
        {
            _step_in s(this);
            std::unique_lock<std::mutex> lock(this->_queue_lock);

            if (this->_queue.empty())
                return false;
            out = this->_queue.back();
            return true;
        }

    private:

        void _assert_dispose() const
        {
            if (this->_disposed)
                throw std::runtime_error("Disposed queue.");
        }

        std::queue<T> _queue;
        std::mutex _queue_lock;
        std::condition_variable _new_element;
        std::condition_variable _element_popped;
        std::condition_variable _empty;
        bool _disposed;
        bool _sized;
        size_type _max_size;

        unsigned long long _in_count;
        std::mutex _in_lock;
        std::condition_variable _in_cond_var;

        cross_thread_queue(const cross_thread_queue&) = delete;
        cross_thread_queue& operator=(const cross_thread_queue&) = delete;
        cross_thread_queue(cross_thread_queue&&) = delete;
        cross_thread_queue& operator=(cross_thread_queue&&) = delete;
    };
}
