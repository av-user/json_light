//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include <string>
#include "json_element.h"
class json_string : public json_element {
public:
	json_string();
	json_string(std::string str);
	json_string(const std::string* pstr);
	virtual std::string get_string() const;
private:
	std::string _string;
};

