#pragma once

#include <sstream>
#include "Serialize.hpp"
#include "tinyxml2.hpp"
#include "Attributes.hpp"

class EXPORT XMLNodeSerializer
{
public:
    XMLNodeSerializer(tinyxml2::XMLNode* node);

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

        XMLNodeSerializer childs(element);

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

        XMLNodeSerializer childs(element);

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

        XMLNodeSerializer childs(element);

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

        XMLNodeSerializer child(element);
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
        XMLIteratingSerializer it(this->_node);

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
        XMLIteratingSerializer it(this->_node);
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
                    get_all(XMLNodeSerializer(element), "item", data);
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
            get_all(XMLNodeSerializer(element), "item", data);
            return true;
        }
        return false;
    }

    template<typename T>
    bool _get_serializable(tinyxml2::XMLElement* element, T& data)
    {
        if (element != nullptr)
            return data.deserialize(XMLNodeSerializer(element));
        return false;
    }

    tinyxml2::XMLNode* _node;
};

class EXPORT XMLIteratingSerializer : public XMLNodeSerializer
{
public:
    XMLIteratingSerializer(tinyxml2::XMLNode* node);
    ~XMLIteratingSerializer() = default;

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

class EXPORT XMLSerializer : public XMLNodeSerializer
{
public:
    XMLSerializer(const std::string& filename);
    ~XMLSerializer();

    void save();
    void load();

private:
    std::string* _filename;
    tinyxml2::XMLDocument* _doc;

    XMLSerializer(const XMLSerializer&) = delete;
    XMLSerializer& operator=(const XMLSerializer&) = delete;
    XMLSerializer(XMLSerializer&&) = delete;
    XMLSerializer& operator=(XMLSerializer&&) = delete;
};