//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include "json_array.h"
#include "json_string.h"
#include "json_number.h"

json_array::json_array() : json_element(json_element::TypeArray) {
}
json_array::~json_array() {
	for (std::vector<json_element*>::iterator it = _children.begin(); it != _children.end(); it++) {
		delete* it;
	}
}
void json_array::add(json_element* p_element) {
	this->_children.push_back(p_element);
}
void json_array::add(const std::string* pstr) {
	json_string* p_jstr_el = new json_string(pstr);
	add(p_jstr_el);
}
void json_array::add_numeric(const std::string* pstr) {
	json_number* p_num_el = new json_number(pstr);
	add(p_num_el);
}
const json_element& json_array::at(int index) const {
	json_element* p_element = NULL;
	if (index < this->_children.size()) {
		p_element = this->_children[index];
	} else {
		throw 108;
	}
	return *p_element;
}

