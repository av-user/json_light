//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include "json_element.h"
#include "json_light.h"

json_element::~json_element(){}

json_element::json_element(json_element::Type type)
: _type	(type)
{}
json_element::json_element(json_element::Type type, std::string jstr)
: _type(type) 
, _jstr(jstr)
{
}
json_element::Type json_element::get_type() const{
	return this->_type;
}
std::string json_element::get_jstring() const{
	return this->_jstr;
}
std::string json_element::get_string(std::string key_str) const {
	throw json_light_exception("call to json_element::get_string() not allowed");
}
std::string json_element::get_string(const char *pkey) const {
	throw json_light_exception("call to json_element::get_string() not allowed");
}
int json_element::get_int(const char* pkey) const {
	throw json_light_exception("call to json_element::get_int() not allowed");
}
const json_array& json_element::get_array(const char* pkey) const {
	throw json_light_exception("call to json_element::get_array() not allowed");
}
const json_object& json_element::get_object(const char* pkey) const {
	throw json_light_exception("call to json_element::get_object() not allowed");
}
