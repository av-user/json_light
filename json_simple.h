//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include "json_element.h"
class json_simple : public json_element {
public:
	enum SimpleType { SimpleNull, SimpleTrue, SimpleFalse };
	json_simple(SimpleType stype);
	SimpleType get_simple_type();
private:
	SimpleType _simple_type;


};

