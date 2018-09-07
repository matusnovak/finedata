#include <ffw/data/datawriter.h>

ffw::DataWriter::DataWriter() {

}

ffw::DataWriter::~DataWriter() {

}

void ffw::DataWriter::add(const Key& key, const Node& value, const Node::Object& attributes) {
    (void)value;
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Key& key, Node::Integer value, const Node::Object& attributes) {
    (void)value;
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Key& key, Node::Float value, const Node::Object& attributes) {
    (void)value;
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Key& key, Node::Boolean value, const Node::Object& attributes) {
    (void)value;
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Key& key, const Node::String& value, const Node::Object& attributes) {
    (void)value;
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Key& key, Node::Null value, const Node::Object& attributes) {
    (void)value;
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::addArray(const Key& key, const Node::Object& attributes) {
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::addObject(const Key& key, const Node::Object& attributes) {
    (void)key;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Node& value, const Node::Object& attributes) {
    (void)value;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(Node::Integer value, const Node::Object& attributes) {
    (void)value;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(Node::Float value, const Node::Object& attributes) {
    (void)value;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(Node::Boolean value, const Node::Object& attributes) {
    (void)value;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(const Node::String& value, const Node::Object& attributes) {
    (void)value;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::add(Node::Null value, const Node::Object& attributes) {
    (void)value;
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::addArray(const Node::Object& attributes) {
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}

void ffw::DataWriter::addObject(const Node::Object& attributes) {
    (void)attributes;
    throw DataWriterException("attributes are not supported");
}