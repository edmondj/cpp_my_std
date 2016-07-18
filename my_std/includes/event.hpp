#pragma once

#include <functional>
#include <list>

template<typename TSender, typename... TArgs>
class event
{
public:
    typedef std::function<void(TSender&, TArgs...)> handler;

private:
    std::list<handler> _handlers;

public:
    void add(handler&& h)
    {
        _handlers.push_back(std::move(h));
    }

    void add(const handler& h)
    {
        _handlers.push_back(h);
    }

private:
    friend TSender;

    void operator()(TSender& sender, TArgs... args)
    {
        for (handler& h : _handlers)
            h(sender, args...);
    }
};