//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include <string>

class json_array;
class json_object;

class json_element {
public:
	virtual ~json_element();
	enum Type { TypeUndefined, TypeNumber, TypeString, TypeArray, TypeObject, TypeSimple };
	Type get_type() const;
	virtual std::string get_jstring() const;
	virtual std::string get_string(std::string key_str) const;
	virtual std::string get_string(const char* pkey) const;
	virtual int get_int(const char* pkey) const;
	//virtual int get_int(const char* pkey) const;
	virtual const json_array& get_array(const char* pkey) const;
	virtual const json_object& get_object(const char* pkey) const;
protected:
	Type _type;
	std::string _key;
	std::string _jstr;
	json_element(Type type);
	json_element(Type type, std::string jstr);
};
