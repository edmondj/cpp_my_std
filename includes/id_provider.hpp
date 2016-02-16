#pragma once

class IDProvider
{
public:
    IDProvider(unsigned int max = 1000);
    IDProvider(const IDProvider& other) = default;
    IDProvider& operator=(const IDProvider& other) = default;
    IDProvider(IDProvider&& other) = default;
    IDProvider& operator=(IDProvider&& other) = default;
    ~IDProvider() = default;

    unsigned int get_next();

private:
    unsigned int _current;
    unsigned int _max;
};