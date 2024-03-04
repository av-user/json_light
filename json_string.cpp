//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include "json_string.h"

json_string::json_string() : json_element (json_element::TypeString)
{}
json_string::json_string(std::string str) 
: json_element(json_element::TypeString)
, _string (str)
{}
json_string::json_string(const std::string *pstr)
	: json_element(json_element::TypeString)
	, _string(*pstr) {
}
std::string json_string::get_string() const {
	return this->_string;
}
