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

#include "json.hpp"
#include "utils.hpp"
#include "my_std_export.hpp"
#include "serialize.hpp"

namespace my_std
{
    class json_serializer;

    class MY_STD_EXPORT_TAG __json_serializer
    {
    private:
        __json_serializer(nlohmann::json& json)
            : _json(json)
        {}

        __json_serializer(const __json_serializer&) = delete;

        template<typename T, bool Convert = !std::is_arithmetic<std::remove_reference<T>::type>::value && !std::is_same<std::string, T>::value>
        struct __io
        {
            static void put(nlohmann::json& json, const T& value)
            {
                json = value;
            }

            static bool get(const nlohmann::json& json, T& data)
            {
                data = json;
                return true;
            }
        };

        template<typename T>
        struct __io<T, true>
        {
            static void put(nlohmann::json& json, T value)
            {
                json = to_string(value);
            }

            static bool get(nlohmann::json& json, T& data)
            {
                return from_string(json, data);
            }
        };

        nlohmann::json& _json;

        friend json_serializer;

    public:
        template<typename T>
        void put_io(const std::string&, const T& value)
        {
            __io<T>::put(this->_json, value);
        }

        template<typename T>
        bool get_io(const std::string&, T& value)
        {
            return __io<T>::get(this->_json, value);
        }

        template<typename T>
        void put_array(const std::string& name, const T& value, size_t size)
        {
            for (size_t i = 0; i < size; i++)
            {
                nlohmann::json js;
                __json_serializer s(js);

                put(s, name, value[i]);
                this->_json.push_back(std::move(js));
            }
        }

        template<typename T>
        bool get_array(const std::string& name, T& value, size_t size)
        {
            if (this->_json.size() != size)
                return false;
            for (size_t i = 0; i < size; i++)
            {
                __json_serializer s(this->_json[i]);
                if (!get(s, name, value[i]))
                    return false;
            }
            return true;
        }

        template<typename T>
        void put_container(const std::string& name, const T& data)
        {
            for (const auto& d : data)
            {
                nlohmann::json js;
                __json_serializer s(js);

                put(s, name, d);
                this->_json.push_back(std::move(js));
            }
        }

        template<typename T>
        bool get_container(const std::string& name, T& data)
        {
            for (nlohmann::json& js : this->_json)
            {
                __json_serializer s(js);
                typename T::value_type value;
                if (!get(s, name, value))
                    return false;
                data.push_back(std::move(value));
            }
            return true;
        }

        template<typename T>
        void put_serializable(const std::string&, const T& data)
        {
            json_serializer js;

            data.serialize(js);
            this->_json = std::move(js._json);
        }

        template<typename T>
        bool get_serializable(const std::string&, T& data)
        {
            json_serializer js;

            js._json = this->_json;
            return data.deserialize(js);
        }
    };

    class MY_STD_EXPORT_TAG json_serializer
    {
    public:
        std::string dump(const int indent = -1) const;
        void parse(const std::string& json);
        void parse(std::istream& json);

        template<typename T>
        void put(const std::string& name, const T& value)
        {
            __json_serializer s(this->_json[name]);
            my_std::put(s, name, value);
        }

        template<typename T>
        bool get(const std::string& name, T& value) const
        {
            auto it = this->_json.find(name);
            if (it == this->_json.end())
                return false;
            __json_serializer s(const_cast<nlohmann::json&>(*it));
            return my_std::get(s, name, value);
        }

    private:
        nlohmann::json _json;

        friend __json_serializer;
    };
}