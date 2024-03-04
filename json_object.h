//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include <map>
#include <string>
#include "json_element.h"
class json_object : public json_element {
public:
	json_object();
	virtual ~json_object();
	void add(std::string keystr, json_element* p_element);
	void add(std::string keystr, const std::string* pstr);
	void add_numeric(std::string keystr, const std::string* pstr);
	json_element* get_child(std::string key_str);
	const json_element& get_child_ref(std::string key_str) const;
	const json_object& obj(std::string key_str) const;
	const json_array& arr(std::string key_str) const;
	std::string get_string(std::string key_str) const;
	virtual std::string get_string(const char* pkey) const;
	virtual int get_int(const char* pkey) const;
	virtual const json_array& get_array(const char* pkey) const;
	virtual const json_object& get_object(const char* pkey) const;
private:
	std::map<std::string, json_element*> _children;
};
