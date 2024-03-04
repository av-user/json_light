//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include "json_object.h"
#include "json_string.h"
#include "json_number.h"
#include "json_array.h"
#include "json_light.h"

json_object::json_object() : json_element(json_element::TypeObject){
}
json_object::~json_object() {
	for (std::map<std::string, json_element*>::iterator it = _children.begin(); it != _children.end(); it++) {
		delete it->second;
	}
}
void json_object::add(std::string keystr, json_element* p_element) {
	if (this->_children.find(keystr) != this->_children.end()) {
		throw json_light_exception("json_object::add() duplicate key");
	} else {
		this->_children[keystr] = p_element;
	}
}
void json_object::add(std::string keystr, const std::string *pstr) {
	json_string* p_jstr_el = new json_string(pstr);
	add(keystr, p_jstr_el);
}
void json_object::add_numeric(std::string keystr, const std::string *pstr) {
	json_number *p_num_el = new json_number(pstr);
	add(keystr, p_num_el);
}
json_element* json_object::get_child(std::string key_str) {
	json_element *p_element = NULL;
	if (this->_children.find(key_str) != this->_children.end()) {
		p_element = this->_children[key_str];
	} else {
		char buffer[200];
		::sprintf_s(buffer, "json_object::%s() no child with key '%s'", __func__, key_str.c_str());
		throw json_light_exception(buffer);
	}
	return p_element;
}
const json_element& json_object::get_child_ref(std::string key_str) const {
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(key_str);
	if (cit != this->_children.end()) {
		return *(cit->second);
	} else {
		char buffer[200];
		::sprintf_s(buffer, "%s() no child with key '%s'", __func__, key_str.c_str());
		throw json_light_exception(buffer);
	}
}
const json_object& json_object::obj(std::string key_str) const {
	char buffer[200];
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(key_str);
	if (cit != this->_children.end()) {
		if (cit->second->get_type() == TypeObject) {
			return *dynamic_cast<const json_object*>(cit->second);
		} else {
			::sprintf_s(buffer, "json_object::%s() element with key '%s' is not of type TypeObject", __func__, key_str.c_str());
			throw json_light_exception(buffer);
		}
	} else {
		::sprintf_s(buffer, "json_object::%s() no child with key '%s'", __func__, key_str.c_str());
		throw json_light_exception(buffer);
	}
}
const json_array& json_object::arr(std::string key_str) const {
	char buffer[200];
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(key_str);
	if (cit != this->_children.end()) {
		if (cit->second->get_type() == TypeArray) {
			return *dynamic_cast<const json_array*>(cit->second);
		} else {
			::sprintf_s(buffer, "json_object::%s() element with key '%s' is not of type TypeArray", __func__, key_str.c_str());
			throw json_light_exception(buffer);
		}
	} else {
		char buffer[200];
		::sprintf_s(buffer, "%s() no child with key '%s'", __func__, key_str.c_str());
		throw json_light_exception(buffer);
	}
}
std::string json_object::get_string(std::string key_str) const {
	std::string str;
	const json_element* p_element = NULL;
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(key_str);
	if (cit != this->_children.end()) {
		p_element = cit->second;
		if (p_element->get_type() == json_element::TypeString) {
			const json_string *pstr = dynamic_cast<const json_string*>(p_element);
			str = pstr->get_string();
		}
	} else {
		char buffer[200];
		::sprintf_s(buffer, "%s() key '%s' does not exist", key_str.c_str(), __func__);
		throw json_light_exception(buffer);
	}
	return str;
}
std::string json_object::get_string(const char* pkey) const {
	std::string str;
	const json_element* p_element = NULL;
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(pkey);
	if (cit != this->_children.end()) {
		p_element = cit->second;
		if (p_element->get_type() == json_element::TypeString) {
			const json_string* pstr = dynamic_cast<const json_string*>(p_element);
			str = pstr->get_string();
		} else {
			throw json_light_exception("json_object::get_string() this key belongs to object that is not of type string");
		}
	} else {
		throw json_light_exception("json_object::get_string() key does not exist");
	}
	return str;
}
//std::string json_object::get_string(const char *pkey) const {
//	std::string str = get_string(pkey);
//	return str;
//}
int json_object::get_int(const char* pkey) const {
	int iret = 0;
	const json_number* p_json_number = NULL;
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(pkey);
	if (cit != this->_children.end()) {
		p_json_number = dynamic_cast<json_number*>(cit->second);
		if (p_json_number != NULL) {
			iret = p_json_number->get_int();
		} else {
			throw json_light_exception("json_object::get_int() this key belongs to object that is not of type number");
		}
	} else {
		throw json_light_exception("json_object::get_int() key does not exist");
	}
	return iret;
}
const json_array& json_object::get_array(const char* pkey) const {
	const json_element* p_element = NULL;
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(pkey);
	if (cit != this->_children.end()) {
		p_element = cit->second;
		if (p_element->get_type() == json_element::TypeArray) {
			return *dynamic_cast<const json_array*>(p_element);
		} else {
			throw 104;
		}
	} else {
		throw 105;
	}
}
const json_object& json_object::get_object(const char* pkey) const {
	const json_element* p_element = NULL;
	std::map<std::string, json_element*>::const_iterator cit = this->_children.find(pkey);
	if (cit != this->_children.end()) {
		p_element = cit->second;
		if (p_element->get_type() == json_element::TypeObject) {
			return *dynamic_cast<const json_object*>(p_element);
		} else {
			throw 106;
		}
	} else {
		throw 107;
	}
}