#include <cassert>

#include "node.h"
#include "util.h"

namespace keson
{
	const Node Node::NULL_NODE;

	Node::Node() {}
	Node::Node(std::string value)                            : _value(std::move(value)) {}
	Node::Node(const char* value)                            : _value(std::string(value)) {}
	Node::Node(Vector value)                                 : _value(std::move(value)) {}
	Node::Node(Map value)                                    : _value(std::move(value)) {}
	Node::Node(int8_t value)                                 : _value(to_string(value)) {}
	Node::Node(uint8_t value)                                : _value(to_string(value)) {}
	Node::Node(int16_t value)                                : _value(to_string(value)) {}
	Node::Node(uint16_t value)                               : _value(to_string(value)) {}
	Node::Node(int32_t value)                                : _value(to_string(value)) {}
	Node::Node(uint32_t value)                               : _value(to_string(value)) {}
	Node::Node(int64_t value)                                : _value(to_string(value)) {}
	Node::Node(uint64_t value)                               : _value(to_string(value)) {}
	Node::Node(float value)                                  : _value(to_string(value)) {}
	Node::Node(double value)                                 : _value(to_string(value)) {}
	Node::Node(bool value)                                   : _value(to_string(value)) {}
#if KESON_ENABLE_WSTRING
	Node::Node(const wchar_t* value)                         : _value(to_utf8(value)) {}
	Node::Node(const std::wstring& value)                    : _value(to_utf8(value)) {}
#endif

	bool Node::isNull() const                                { return std::holds_alternative<Null>(_value); }
	bool Node::isAtom() const                                { return std::holds_alternative<Atom>(_value); }
	bool Node::isVector() const                              { return std::holds_alternative<Vector>(_value); }
	bool Node::isMap() const                                 { return std::holds_alternative<Map>(_value); }

	const Node::Atom& Node::atom() const {
		return std::get<Atom>(_value);
	}

	Node::Atom& Node::atom() {
		if (isNull()) { _value = Atom(); }
		return std::get<Atom>(_value);
	}

	std::string Node::value_or(std::string fallback) const   { return !isAtom() ? fallback : atom(); }
    int8_t      Node::value_or(int8_t fallback) const        { return !isAtom() ? fallback : (int8_t)(*this); }
    uint8_t     Node::value_or(uint8_t fallback) const       { return !isAtom() ? fallback : (uint8_t)(*this); }
    int16_t     Node::value_or(int16_t fallback) const       { return !isAtom() ? fallback : (int16_t)(*this); }
    uint16_t    Node::value_or(uint16_t fallback) const      { return !isAtom() ? fallback : (uint16_t)(*this); }
    int32_t     Node::value_or(int32_t fallback) const       { return !isAtom() ? fallback : (int32_t)(*this); }
    uint32_t    Node::value_or(uint32_t fallback) const      { return !isAtom() ? fallback : (uint32_t)(*this); }
    int64_t     Node::value_or(int64_t fallback) const       { return !isAtom() ? fallback : (int64_t)(*this); }
    uint64_t    Node::value_or(uint64_t fallback) const      { return !isAtom() ? fallback : (uint64_t)(*this); }
    float       Node::value_or(float fallback) const         { return !isAtom() ? fallback : (float)(*this); }
    double      Node::value_or(double fallback) const        { return !isAtom() ? fallback : (double)(*this); }
    bool        Node::value_or(bool fallback) const          { return !isAtom() ? fallback : (bool)(*this); }
#if KESON_ENABLE_WSTRING
	std::wstring Node::value_or(std::wstring fallback) const { return !isAtom() ? fallback : from_utf8(atom()); }
#endif

	Node::operator std::string() const                       { return atom(); }
	Node::operator int8_t() const                            { return from_string<int8_t>(atom());   }
	Node::operator uint8_t() const                           { return from_string<uint8_t>(atom());  }
	Node::operator int16_t() const                           { return from_string<int16_t>(atom());  }
	Node::operator uint16_t() const                          { return from_string<uint16_t>(atom()); }
	Node::operator int32_t() const                           { return from_string<int32_t>(atom());  }
	Node::operator uint32_t() const                          { return from_string<uint32_t>(atom()); }
	Node::operator int64_t() const                           { return from_string<int64_t>(atom());  }
	Node::operator uint64_t() const                          { return from_string<uint64_t>(atom()); }
	Node::operator float() const                             { return from_string<float>(atom());    }
	Node::operator double() const                            { return from_string<double>(atom());   }
	Node::operator bool() const                              { return from_string<bool>(atom());     }
#if KESON_ENABLE_WSTRING
	Node::operator std::wstring() const                      { return from_utf8(atom()); }
#endif

	void Node::operator=(std::string value)                  { _value = std::move(value); }
	void Node::operator=(const char* value)                  { _value = std::string(value); }
	void Node::operator=(int8_t value)                       { _value = to_string(value); }
	void Node::operator=(uint8_t value)                      { _value = to_string(value); }
	void Node::operator=(int16_t value)                      { _value = to_string(value); }
	void Node::operator=(uint16_t value)                     { _value = to_string(value); }
	void Node::operator=(int32_t value)                      { _value = to_string(value); }
	void Node::operator=(uint32_t value)                     { _value = to_string(value); }
	void Node::operator=(int64_t value)                      { _value = to_string(value); }
	void Node::operator=(uint64_t value)                     { _value = to_string(value); }
	void Node::operator=(float value)                        { _value = to_string(value); }
	void Node::operator=(double value)                       { _value = to_string(value); }
	void Node::operator=(bool value)                         { _value = to_string(value); }
#if KESON_ENABLE_WSTRING
	void Node::operator=(const std::wstring& value)          { _value = to_utf8(value); }
	void Node::operator=(const wchar_t* value)               { _value = to_utf8(value); }
#endif

	const Node::Vector& Node::vector() const {
		return std::get<Vector>(_value);
	}

	Node::Vector& Node::vector() {
		if (isNull()) { _value = Vector(); }
		return std::get<Vector>(_value);
	}

	size_t Node::length() const {
		if (isVector()) {
			return vector().size();
		}
		else if (isNull()) {
			return 0;
		}
		else {
			return 1;
		}
	}

	const Node* Node::begin() const {
		if (isVector()) {
			return vector().data();
		}
		else if (isNull()) {
			return nullptr;
		}
		else {
			return this;
		}
	}

	Node* Node::begin() {
		if (isVector()) {
			return vector().data();
		}
		else if (isNull()) {
			return nullptr;
		}
		else {
			return this;
		}
	}

	const Node* Node::end() const {
		if (isVector()) {
			return vector().data() + vector().size();
		}
		else if (isNull()) {
			return nullptr;
		}
		else {
			return this + 1;
		}
	}

	Node* Node::end() {
		if (isVector()) {
			return vector().data() + vector().size();
		}
		else if (isNull()) {
			return nullptr;
		}
		else {
			return this + 1;
		}
	}

	Node& Node::operator[](size_t pos) {
		if (vector().size() <= pos) {
			vector().resize(pos + 1);
		}
		return vector()[pos];
	}

	const Node& Node::operator[](size_t pos) const {
		return vector().at(pos);
	}

	void Node::push_back(Node node) {
		vector().emplace_back(std::move(node));
	}

	Node& Node::push_back() {
		vector().emplace_back(Node());
		return vector().back();
	}

	const Node::Map& Node::map() const {
		return std::get<Map>(_value);
	}

	Node::Map& Node::map() {
		if (isNull()) { _value = Map(); }
		return std::get<Map>(_value);
	}

	const Node& Node::operator[](const std::string& key) const {
		if (isMap()) {
			auto& v = map();
			auto it = v.find(key);
			if (it != v.end()) {
				return it->second;
			}
		}
		return Node::NULL_NODE;
	}

	Node& Node::operator[](const std::string& key) {
		return map()[key];
	}

	Node& Node::operator[](const char* key) {
		return (*this)[std::string(key)];
	}

	const Node& Node::operator[](const char* key) const {
		return (*this)[std::string(key)];
	}

	void swap(Node& a, Node& b) {
		std::swap(a._value, b._value);
	}
}

void std::swap(keson::Node& a, keson::Node& b) {
	keson::swap(a, b);
}
