//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include "json_simple.h"

json_simple::json_simple(SimpleType stype) 
: json_element (json_element::TypeSimple)
, _simple_type(stype)
{}
json_simple::SimpleType json_simple::get_simple_type() {
	return _simple_type;
}

