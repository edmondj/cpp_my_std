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
#include "file_system.hpp"
#include "xml_serializer.hpp"

using namespace std;
using namespace tinyxml2;

my_std::xml_serializer::xml_serializer(const std::string& filename)
    : xml_node_serializer(new XMLDocument()), _filename(new string(filename)), _doc((XMLDocument*)_node)
{}

my_std::xml_serializer::~xml_serializer()
{
    delete this->_doc;
    delete this->_filename;
}

void my_std::xml_serializer::save()
{
    this->_doc->SaveFile(this->_filename->c_str());
}

void my_std::xml_serializer::load()
{
    this->_doc->LoadFile(this->_filename->c_str());
}

my_std::xml_node_serializer::xml_node_serializer(tinyxml2::XMLNode* node)
    : _node(node)
{
}

my_std::xml_iterating_serializer::xml_iterating_serializer(tinyxml2::XMLNode* node)
    : xml_node_serializer(node), _current(nullptr)
{ }

bool my_std::xml_iterating_serializer::moveNext(const string& name)
{
    if (this->_current == nullptr)
        this->_current = this->_node->FirstChildElement(name.c_str());
    else
        this->_current = this->_current->NextSiblingElement(name.c_str());
    return this->_current != nullptr;
}
