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

#include <iterator>
#include <utility>

namespace my_std
{
    template<typename T>
    class circular_buffer
    {
    public:
        typedef T value_type;
        typedef std::ptrdiff_t size_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;

        template<typename TBuffer, typename TPointer, typename TReference>
        class __iterator : std::iterator<std::forward_iterator_tag, value_type, size_type, TPointer, TReference>
        {
        public:
            __iterator(TBuffer* buffer, TPointer offset)
                : _buffer(buffer), _offset(offset)
            { }

            __iterator(const __iterator& other)
                : _buffer(other._buffer), _offset(other._offset)
            { }

            __iterator& operator=(const __iterator& other)
            {
                this->_buffer = other._buffer;
                this->_offset = other._offset;
                return *this;
            }

            ~__iterator()
            { }

            void swap(__iterator& other)
            {
                std::swap(this->_buffer, other._buffer);
                std::swap(this->_offset, other._offset);
            }

            bool operator==(const __iterator& other) const
            {
                return this->_buffer == other._buffer && this->_offset == other._offset;
            }

            TReference operator*() const
            {
                return *this->_offset;
            }

            __iterator& operator++()
            {
                this->_buffer->advance(this->_offset);
                if (this->_offset == this->_buffer->_writePtr)
                    this->_offset = nullptr;
                return *this;
            }

            bool operator!=(const __iterator& other) const
            {
                return !(*this == other);
            }

            TPointer operator->() const
            {
                return this->_offset;
            }

            __iterator operator++(int)
            {
                __iterator sent(*this);

                ++(*this);
                return sent;
            }

        private:
            TBuffer* _buffer;
            TPointer _offset;
        };

        typedef __iterator<circular_buffer, pointer, reference> iterator;
        typedef __iterator<const circular_buffer, const_pointer, const_reference> const_iterator;

        circular_buffer(size_t size)
            : circular_buffer(size, new T[size])
        { }

        circular_buffer(const circular_buffer& other)
            : circular_buffer(other._totalSize, new T[size])
        {
            for (const T& value : other)
                this->push(value);
        }

        circular_buffer(circular_buffer&& other)
            : circular_buffer(other._totalSize, other._buffer, other._readPtr, other._writePtr)
        {
            other._buffer = nullptr;
        }

        ~circular_buffer()
        {
            delete[] this->_buffer;
        }

        circular_buffer& operator=(const circular_buffer& other)
        {
            if (this != &other)
            {
                delete[] this->_buffer;
                this->_totalSize = other._totalSize;
                this->_buffer = new T[this->_totalSize];
                for (const T& value : other)
                    this->push(value);
            }
            return *this;
        }

        circular_buffer& operator=(circular_buffer&& other)
        {
            if (this != &other)
            {
                delete[] this->_buffer;
                this->_totalSize = other._totalSize;
                this->_buffer = other._buffer;
                this->_readPtr = other._readPtr;
                this->_writePtr = other._writePtr;
            }
            return *this;
        }

        reference front()
        {
            return *this->_readPtr;
        }

        const_reference front() const
        {
            return *this->_readPtr;
        }

        reference back()
        {
            if (this->_writePtr != this->_buffer) // if we are not at the beginning of our buffer
                return *(this->_writePtr - 1); // get the one behind the write pointer
            return this->_buffer[this->_totalSize - 1]; // get the last spot in the buffer
        }

        const_reference back() const
        {
            if (this->_writePtr != this->_buffer) // if we are not at the beginning of our buffer
                return *(this->_writePtr - 1); // get the one behind the write pointer
            return this->_buffer[this->_totalSize - 1]; // get the last spot in the buffer
        }

        bool empty() const
        {
            // Nothing was ever written
            return this->_writePtr == nullptr;
        }

        bool full() const
        {
            return this->_writePtr == this->_readPtr;
        }

        size_type size() const
        {
            if (this->_writePtr == nullptr)
                return 0;
            else if (this->_writePtr > this->_readPtr)
                return this->_writePtr - this->_readPtr;
            return this->_totalSize - (this->_readPtr - this->_writePtr);
        }

        size_type capacity() const
        {
            return this->_totalSize;
        }

        void push(const value_type& value)
        {
            // If first push
            if (this->_writePtr == nullptr)
                this->_writePtr = this->_buffer;
            else if (this->_writePtr == this->_readPtr) // if writing on the next reading spot
                this->advance(this->_readPtr);
            *this->_writePtr = value;
            this->advance(this->_writePtr);
        }

        void pop()
        {
            this->advance(this->_readPtr);
        }

        iterator begin()
        {
            return iterator(this, this->_readPtr);
        }

        const_iterator begin() const
        {
            return this->cbegin();
        }

        const_iterator cbegin() const
        {
            return const_iterator(this, this->_readPtr);
        }

        iterator end()
        {
            return iterator(this, nullptr);
        }

        const_iterator end() const
        {
            return this->cend();
        }

        const_iterator cend() const
        {
            return const_iterator(this, nullptr);
        }

    private:
        circular_buffer(size_t size, T* buffer, T* readPtr = nullptr, T* writePtr = nullptr)
            : _totalSize(size), _buffer(buffer), _readPtr(readPtr), _writePtr(writePtr)
        {
            if (this->_readPtr == nullptr)
                this->_readPtr = buffer;
        }

        void advance(pointer& ptr) const
        {
            ++ptr;
            if (ptr - this->_buffer >= this->_totalSize)
                ptr = this->_buffer;
        }

        void advance(const_pointer& ptr) const
        {
            ++ptr;
            if (ptr - this->_buffer >= this->_totalSize)
                ptr = this->_buffer;
        }

        size_type _totalSize;
        pointer _buffer;
        pointer _readPtr;
        pointer _writePtr;
    };

    template<typename T>
    void swap(typename circular_buffer<T>::iterator& t, typename circular_buffer<T>::iterator& u)
    {
        t.swap(u);
    }

    template<typename T>
    void swap(typename circular_buffer<T>::const_iterator& t, typename circular_buffer<T>::const_iterator& u)
    {
        t.swap(u);
    }
}