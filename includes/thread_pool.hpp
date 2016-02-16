#pragma once

#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <queue>
#include <atomic>
#include <vector>

template<typename TRet, typename... TArgs>
class ThreadPool
{
public:
    ThreadPool(unsigned int nbThread = 0)
        : _stop(false), _pause(false), _idleCount(0), _threads(0)
    {
        this->set_nb_thread(nbThread);
    }

    ~ThreadPool()
    {
        this->stop_threads();
    }

    void set_nb_thread(unsigned int nbThread)
    {
        this->stop_threads();
        this->_threads.resize(nbThread);
        for (std::thread& th : this->_threads)
            th = std::thread(&ThreadPool::threadLoop, this);
    }

    void get_nb_thread() const
    {
        return this->_threads.size();
    }

    void on_done(const std::function<void(TRet)>& handler)
    {
        this->_doneHandler = handler;
    }

    void on_except(const std::function<void(const std::exception&)>& handler)
    {
        this->_exceptHandler = handler;
    }

    void add_task(const std::function<TRet(TArgs...)>& f, TArgs... args)
    {
        std::unique_lock<std::mutex> lock(this->_tasksLock);

        this->_tasks.push(std::bind(f, args...));
        this->_newTask.notify_all();
    }

    void wait_all()
    {
        std::unique_lock<std::mutex> lock(this->_tasksLock);

        while (!this->_stop && this->_idleCount < (int)this->_threads.size())
            this->_idleThread.wait(lock);
    }

    void cancel_all()
    {
        std::unique_lock<std::mutex> lock(this->_tasksLock);
        this->_tasks = std::queue<std::function<TRet()>>();
    }

    void pause()
    {
        this->_pause = true;
    }

    void resume()
    {
        this->_pause = false;
        this->_resumeThreads.notify_all();
    }

private:
    std::function<TRet()> extractTask()
    {
        std::unique_lock<std::mutex> lock(this->_tasksLock);
        std::function<TRet()> sent;

        while (!this->_stop && this->_pause)
            this->_resumeThreads.wait(lock);
        while (!this->_stop && this->_tasks.empty())
        {
            this->_idleThread.notify_all();
            ++this->_idleCount;
            this->_newTask.wait(lock);
            --this->_idleCount;
        }
        if (!this->_tasks.empty())
        {
            sent = this->_tasks.front();
            this->_tasks.pop();
        }
        return move(sent);
    }

    void threadLoop()
    {
        std::function<TRet()> task;
        do
        {
            task = move(this->extractTask());
            if (task)
            {
                try
                {
                    TRet ret(task());
                    if (this->_doneHandler)
                        this->_doneHandler(ret);
                }
                catch (std::exception& except)
                {
                    if (this->_exceptHandler)
                        this->_exceptHandler(except);
                }
            }
        } while (!this->_stop && task);
    }

    void stop_threads()
    {
        this->_stop = true;
        this->_newTask.notify_all();
        this->_idleThread.notify_all();
        this->_resumeThreads.notify_all();
        for (std::thread& th : this->_threads)
            th.join();
        this->_threads.clear();
        this->_stop = false;
    }

    std::atomic<bool> _stop;
    std::atomic<bool> _pause;
    std::atomic<int> _idleCount;
    std::vector<std::thread> _threads;
    std::queue<std::function<TRet()>> _tasks;
    std::mutex _tasksLock;
    std::condition_variable _newTask;
    std::condition_variable _idleThread;
    std::condition_variable _resumeThreads;
    std::function<void(TRet)> _doneHandler;
    std::function<void(const std::exception& except)> _exceptHandler;

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
};