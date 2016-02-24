#pragma once

#include "my_std_export.hpp"

namespace my_std
{
    //class MY_STD_EXPORT_TAG write_oriented_rw_mutex
    //{
    //public:
    //    write_oriented_rw_mutex();
    //    ~write_oriented_rw_mutex();

    //    void read_lock();
    //    bool read_try_lock();
    //    void read_unlock();

    //    void write_lock();
    //    bool write_try_lock();
    //    void write_unlock();

    //    void lock();
    //    bool try_lock();
    //    void unlock();

    //private:
    //    write_oriented_rw_mutex(const write_oriented_rw_mutex&) = delete;
    //    write_oriented_rw_mutex& operator=(const write_oriented_rw_mutex&) = delete;
    //    write_oriented_rw_mutex(write_oriented_rw_mutex&&) = delete;
    //    write_oriented_rw_mutex& operator=(write_oriented_rw_mutex&&) = delete;

    //    struct _fields_t;
    //    _fields_t* _fields;
    //};

    class MY_STD_EXPORT_TAG read_oriented_rw_mutex
    {
    public:
        read_oriented_rw_mutex();
        ~read_oriented_rw_mutex();

        void read_lock();
        bool read_try_lock();
        void read_unlock();

        void write_lock();
        bool write_try_lock();
        void write_unlock();

        void lock();
        bool try_lock();
        void unlock();

    private:
        read_oriented_rw_mutex(const read_oriented_rw_mutex&) = delete;
        read_oriented_rw_mutex& operator=(const read_oriented_rw_mutex&) = delete;
        read_oriented_rw_mutex(read_oriented_rw_mutex&&) = delete;
        read_oriented_rw_mutex& operator=(read_oriented_rw_mutex&&) = delete;

        struct _fields_t;
        _fields_t* _fields;
    };

    typedef read_oriented_rw_mutex read_write_mutex;
}