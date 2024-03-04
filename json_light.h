//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include <vector>
#include <string>
#include "json_object.h"

//#define DEBUG_PARSING

class json_element;
class json_number;
class json_string;

class json_light {
public:
	json_light();
	json_light(const char* pjsonstr);
	virtual ~json_light();
	void parse(const char* pjsonstr);
	void clear();
	int get_int(std::string path_str);
	std::string get_string(std::string path_str);
	const json_object& obj(const char *pkey_str) const;
	const json_array& arr(const char* pkey_str) const;
	json_element::Type get_root_type() const;
	const json_element& get_root_ref() const;
private:
	enum State { StateInitial, StateObject, StateKVP, StateArray, StateNumeric, StateString, StateKey, StateValue, StateHappyend, StateFailure };
	typedef struct StateContent {
		State _state;
		json_element* _p_element;
		std::string _key;
	} StateContent;
	static const char _whitespace[];
	static const char _digits[];
	static const char _numerics[];

	StateContent _state_content;
	std::vector<StateContent> _content_stack;
	std::string _json_string;
	size_t _parse_offset;
	std::string state2str(State state);
	std::string read_string();
	std::string read_numeric();
	void parse_initial();
	void parse_object();
	void parse_array();
	void parse_numeric();
	void parse_string();
	void parse_key();
	void parse_value();
	void parse_kvp();
	void skip_whitespace();
	void skip_colon();
	void skip_to(char char_to);
	void skip_to_oneof(const std::string* p_oneof_str);
	bool is_whitespace();
	bool is_digit();
	bool is_digit(char ch);
	bool is_char_equal(char ch);
#ifdef DEBUG_PARSING
	void where_i_am();
#endif
};

class json_light_exception {
public:
	json_light_exception(const std::string& msg_str) : _msg(msg_str) {}
	~json_light_exception() {}

	std::string message() const { return(_msg); }
private:
	std::string _msg;
};