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