#include <mutex>
#include <condition_variable>
#include "read_write_mutex.hpp"

using namespace std;

struct my_std::read_oriented_rw_mutex::_fields_t
{
    _fields_t()
        : nb_readers(0), writing(false)
    {}

    std::mutex m;
    std::condition_variable c;
    int nb_readers;
    bool writing;
};

my_std::read_oriented_rw_mutex::read_oriented_rw_mutex()
    : _fields(new _fields_t())
{}

my_std::read_oriented_rw_mutex::~read_oriented_rw_mutex()
{
    delete _fields;
}

void my_std::read_oriented_rw_mutex::read_lock()
{
    unique_lock<mutex> lock(this->_fields->m);

    while (this->_fields->writing)
        this->_fields->c.wait(lock);
    ++this->_fields->nb_readers;
}

bool my_std::read_oriented_rw_mutex::read_try_lock()
{
    unique_lock<mutex> lock(this->_fields->m);
    bool sent = !this->_fields->writing;

    ++this->_fields->nb_readers;
    return sent;
}

void my_std::read_oriented_rw_mutex::read_unlock()
{
    unique_lock<mutex> lock(this->_fields->m);

    --this->_fields->nb_readers;
    if (this->_fields->nb_readers == 0) // Wake up writing threads
        this->_fields->c.notify_all();
}

void my_std::read_oriented_rw_mutex::write_lock()
{
    unique_lock<mutex> lock(this->_fields->m);

    while (this->_fields->writing || this->_fields->nb_readers > 0) // While someone is already there
        this->_fields->c.wait(lock);
    this->_fields->writing = true;
}

bool my_std::read_oriented_rw_mutex::write_try_lock()
{
    unique_lock<mutex> lock(this->_fields->m);

    if (this->_fields->writing || this->_fields->nb_readers > 0)
        return false;
    this->_fields->writing = true;
    return true;
}

void my_std::read_oriented_rw_mutex::write_unlock()
{
    unique_lock<mutex> lock(this->_fields->m);

    this->_fields->writing = false;
    this->_fields->c.notify_all();
}

void my_std::read_oriented_rw_mutex::lock()
{
    this->write_lock();
}

bool my_std::read_oriented_rw_mutex::try_lock()
{
    return this->write_try_lock();
}

void my_std::read_oriented_rw_mutex::unlock()
{
    this->write_unlock();
}

//struct my_std::read_oriented_rw_mutex::_fields_t
//{
//    _fields_t()
//        : nb_readers(0)
//    {}
//
//    std::mutex readers;
//    std::mutex global;
//    int nb_readers;
//};
//
//my_std::read_oriented_rw_mutex::read_oriented_rw_mutex()
//    : _fields(new _fields_t())
//{}
//
//my_std::read_oriented_rw_mutex::~read_oriented_rw_mutex()
//{
//    delete _fields;
//}
//
//void my_std::read_oriented_rw_mutex::read_lock()
//{
//    unique_lock<mutex> lock(this->_fields->readers);
//
//    ++this->_fields->nb_readers;
//    if (this->_fields->nb_readers == 1) // If I'm the first reader here
//        this->_fields->global.lock();
//}
//
//bool my_std::read_oriented_rw_mutex::read_try_lock()
//{
//    bool sent = true;
//    unique_lock<mutex> lock(this->_fields->readers);
//
//    ++this->_fields->nb_readers;
//    if (this->_fields->nb_readers == 1)
//        sent = this->_fields->global.try_lock();
//    if (!sent)
//        --this->_fields->nb_readers;
//    return sent;
//}
//
//void my_std::read_oriented_rw_mutex::read_unlock()
//{
//    unique_lock<mutex> lock(this->_fields->readers);
//
//    --this->_fields->nb_readers;
//    if (this->_fields->nb_readers == 0) // If I'm the last reader
//        this->_fields->global.unlock();
//}
//
//void my_std::read_oriented_rw_mutex::write_lock()
//{
//    this->_fields->global.lock();
//}
//
//bool my_std::read_oriented_rw_mutex::write_try_lock()
//{
//    return this->_fields->global.try_lock();
//}
//
//void my_std::read_oriented_rw_mutex::write_unlock()
//{
//    this->_fields->global.unlock();
//}
//
//void my_std::read_oriented_rw_mutex::lock()
//{
//    this->write_lock();
//}
//
//bool my_std::read_oriented_rw_mutex::try_lock()
//{
//    return this->write_try_lock();
//}
//
//void my_std::read_oriented_rw_mutex::unlock()
//{
//    this->write_unlock();
//}
