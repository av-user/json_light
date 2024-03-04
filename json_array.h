//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include <vector>
#include "json_element.h"
class json_array : public json_element {
public:
	json_array();
	virtual ~json_array();
	void add(json_element* p_element);
	void add(const std::string* pstr);
	void add_numeric(const std::string* pstr);
	const json_element& at(int index) const;
private:
	std::vector<json_element*> _children;
};

