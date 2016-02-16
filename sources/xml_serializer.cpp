#include "FileSystem.hpp"
#include "XMLSerializer.hpp"

using namespace std;
using namespace tinyxml2;
using namespace FileSystem;

XMLSerializer::XMLSerializer(const std::string& filename)
    : XMLNodeSerializer(new XMLDocument()), _filename(new string(filename)), _doc((XMLDocument*)_node)
{}

XMLSerializer::~XMLSerializer()
{
    delete this->_doc;
    delete this->_filename;
}

void XMLSerializer::save()
{
    this->_doc->SaveFile(this->_filename->c_str());
}

void XMLSerializer::load()
{
    this->_doc->LoadFile(this->_filename->c_str());
}

XMLNodeSerializer::XMLNodeSerializer(tinyxml2::XMLNode* node)
    : _node(node)
{
}

XMLIteratingSerializer::XMLIteratingSerializer(tinyxml2::XMLNode* node)
    : XMLNodeSerializer(node), _current(nullptr)
{ }

bool XMLIteratingSerializer::moveNext(const string& name)
{
    if (this->_current == nullptr)
        this->_current = this->_node->FirstChildElement(name.c_str());
    else
        this->_current = this->_current->NextSiblingElement(name.c_str());
    return this->_current != nullptr;
}
