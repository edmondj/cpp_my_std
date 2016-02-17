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

#include <sstream>
#include "serialize.hpp"
#include "tinyxml2.hpp"
#include "my_std_export.hpp"

namespace my_std
{
    class MY_STD_EXPORT_TAG xml_node_serializer
    {
    public:
        xml_node_serializer(tinyxml2::XMLNode* node);

        template<typename T>
        void put_io(const std::string& name, const T& data)
        {
            tinyxml2::XMLElement* element = this->_node->GetDocument()->NewElement(name.c_str());
            std::ostringstream out;

            out << data;

            element->SetAttribute("value", out.str().c_str());
            this->_node->InsertEndChild(element);
        }

        template<typename T>
        void put_array(const std::string& name, const T& data)
        {
            tinyxml2::XMLElement* element = this->_node->GetDocument()->NewElement(name.c_str());

            element->SetAttribute("size", std::extent<T>::value);
            this->_node->InsertEndChild(element);

            xml_node_serializer childs(element);

            for (size_t i = 0; i < std::extent<T>::value; i++)
            {
                put(childs, "item", data[i]);
            }
        }

        template<typename T, size_t size>
        void put_array(const std::string& name, const std::array<T, size>& data)
        {
            tinyxml2::XMLElement* element = this->_node->GetDocument()->NewElement(name.c_str());

            element->SetAttribute("size", size);
            this->_node->InsertEndChild(element);

            xml_node_serializer childs(element);

            for (size_t i = 0; i < size; i++)
            {
                put(childs, "item", data[i]);
            }
        }

        template<typename T>
        void put_container(const std::string& name, const T& data)
        {
            tinyxml2::XMLElement* element = this->_node->GetDocument()->NewElement(name.c_str());
            element->SetAttribute("size", data.size());
            this->_node->InsertEndChild(element);

            xml_node_serializer childs(element);

            for (const auto& d : data)
            {
                put(childs, "item", d);
            }
        }

        template<typename T>
        void put_serializable(const std::string& name, const T& data)
        {
            tinyxml2::XMLElement* element = this->_node->GetDocument()->NewElement(name.c_str());
            this->_node->InsertEndChild(element);

            xml_node_serializer child(element);
            data.serialize(child);
        }

        template<typename T>
        bool get_io(const std::string& name, T& data)
        {
            return _get_io(this->_node->FirstChildElement(name.c_str()), data);
        }

        template<typename T>
        bool get_array(const std::string& name, T& data)
        {
            return _get_array<T, std::extent<T>::value>(this->_node->FirstChildElement(name.c_str()), data);
        }

        template<typename T, size_t size>
        bool get_array(const std::string& name, std::array<T, size>& data)
        {
            return _get_array<std::array<T, size>, size>(this->_node->FirstChildElement(name.c_str()), data);
        }

        template<typename T>
        size_t get_all_array(const std::string& name, T& data)
        {
            size_t sent = 0;
            xml_iterating_serializer it(this->_node);

            while (it.moveNext(name) && get(it, name, data[sent]))
                ++sent;
            return sent;
        }

        template<typename T>
        bool get_container(const std::string& name, T& data)
        {
            return _get_container(this->_node->FirstChildElement(name.c_str()), data);
        }

        template<typename T>
        size_t get_all_container(const std::string& name, T& data)
        {
            size_t sent = 0;
            xml_iterating_serializer it(this->_node);
            auto inserter = std::back_inserter(data);
            typename T::value_type item;

            while (it.moveNext(name) && get(it, name, item))
            {
                data.push_back(item);
                ++sent;
            }
            return sent;
        }

        template<typename T>
        bool get_serializable(const std::string &name, T& data)
        {
            return _get_serializable(this->_node->FirstChildElement(name.c_str()), data);
        }

    protected:
        template<typename T>
        bool _get_io(tinyxml2::XMLElement* element, T& data)
        {
            if (element != nullptr)
            {
                const char* attr = element->Attribute("value");
                if (attr != nullptr)
                {
                    std::istringstream iss(attr);

                    return !!(iss >> data);
                }
            }
            return false;
        }

        template<typename T, size_t maxSize>
        bool _get_array(tinyxml2::XMLElement* element, T& data)
        {
            if (element != nullptr)
            {
                const char* attr = element->Attribute("size");
                if (attr != nullptr)
                {
                    size_t size = std::stoi(attr);

                    if (size == maxSize)
                    {
                        get_all(xml_node_serializer(element), "item", data);
                        return true;
                    }
                }
            }
            return false;
        }

        template<typename T>
        bool _get_container(tinyxml2::XMLElement* element, T& data)
        {
            if (element != nullptr)
            {
                get_all(xml_node_serializer(element), "item", data);
                return true;
            }
            return false;
        }

        template<typename T>
        bool _get_serializable(tinyxml2::XMLElement* element, T& data)
        {
            if (element != nullptr)
                return data.deserialize(xml_node_serializer(element));
            return false;
        }

        tinyxml2::XMLNode* _node;
    };

    class MY_STD_EXPORT_TAG xml_iterating_serializer : public xml_node_serializer
    {
    public:
        xml_iterating_serializer(tinyxml2::XMLNode* node);
        ~xml_iterating_serializer() = default;

        template<typename T>
        bool get_io(const std::string& name, T& data)
        {
            return _get_io(this->_current, data);
        }

        template<typename T>
        bool get_array(const std::string& name, T& data)
        {
            return _get_array(this->_current, data);
        }

        template<typename T>
        bool get_container(const std::string& name, T& data)
        {
            return _get_container(this->_current, data);
        }

        template<typename T>
        bool get_serializable(const std::string& name, T& data)
        {
            return _get_serializable(this->_current, data);
        }

        bool moveNext(const std::string& name);

    private:
        tinyxml2::XMLElement* _current;
    };

    class MY_STD_EXPORT_TAG xml_serializer : public xml_node_serializer
    {
    public:
        xml_serializer(const std::string& filename);
        ~xml_serializer();

        void save();
        void load();

    private:
        std::string* _filename;
        tinyxml2::XMLDocument* _doc;

        xml_serializer(const xml_serializer&) = delete;
        xml_serializer& operator=(const xml_serializer&) = delete;
        xml_serializer(xml_serializer&&) = delete;
        xml_serializer& operator=(xml_serializer&&) = delete;
    };
}