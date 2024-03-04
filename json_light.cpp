//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include <iostream>
#include <sstream>
#include "json_light.h"
#include "json_string.h"
#include "json_element.h"
#include "json_number.h"
#include "json_object.h"
#include "json_array.h"
#include "json_simple.h"

const char json_light::_whitespace[] = {'\x20','\x09','\x0a','\x0D'};
const char json_light::_digits[] = { '0','1','2','3','4','5','6','7','8','9' };
const char json_light::_numerics[] = { '0','1','2','3','4','5','6','7','8','9','-','.','e','E'};

json_light::json_light() 
: _parse_offset(0)
{
	_state_content._state = json_light::StateInitial;
	_state_content._p_element = NULL;
}

json_light::json_light(const char* pjsonstr)
: _json_string (pjsonstr)
, _parse_offset(0)
{
	_state_content._state = json_light::StateInitial;
	_state_content._p_element = NULL;
}
json_light::~json_light() {
	this->clear();
}
void json_light::clear() {
	_parse_offset = 0;
	for (std::vector<StateContent>::iterator it = _content_stack.begin(); it != _content_stack.end(); it++) {
		delete it->_p_element;
	}
	this->_content_stack.clear();
	_state_content._state = json_light::StateInitial;
	delete _state_content._p_element;
	_state_content._p_element = NULL;
}
bool json_light::is_whitespace() {
	bool white = false;
	for (int c : json_light::_whitespace) {
		if (this->_json_string.at(this->_parse_offset) == c) {
			white = true;
			break;
		}
	}
	return white;
}
bool json_light::is_digit(char ch) {
	return ch >= '0' && ch <= '9';
}
bool json_light::is_digit() {
	char ch = this->_json_string.at(this->_parse_offset);
	return ch >= '0' && ch <= '9';
}
bool json_light::is_char_equal(char ch) {
	char c = this->_json_string.at(this->_parse_offset);
	return ch == c;
}
void json_light::skip_whitespace() {
	while (this->_parse_offset < this->_json_string.size() && is_whitespace()) {
		this->_parse_offset++;
	}
}
void json_light::skip_colon() {
	this->skip_whitespace();
	this->skip_to(':');
	this->_parse_offset++;
	this->skip_whitespace();
}
void json_light::skip_to_oneof(const std::string *p_oneof_str) {
	throw json_light_exception("call to json_light::skip_to_oneof() todo");
}
void json_light::skip_to(char char_to) {
	while (this->_parse_offset < this->_json_string.size() && this->_json_string.at(this->_parse_offset) != char_to) {
		this->_parse_offset++;
	}
	if (!this->_json_string.at(this->_parse_offset) == char_to) {
		throw json_light_exception("call to json_light::skip_to() failed");
	}
}
void json_light::parse_initial() {
	size_t level = this->_content_stack.size();
	this->skip_whitespace();
	if(level == 0) {
		if (this->_parse_offset < this->_json_string.size()) {
			char ch = this->_json_string.at(this->_parse_offset++);
			switch (ch) {
			case '"':
				throw 0;//todo 'STRING'
				break;
			case '{':
				this->_state_content._p_element = new json_object();
				this->_state_content._state = json_light::StateObject;
				break;
			case '[':
				throw 0;//todo
				break;
			case '-':
				this->_parse_offset--;
				this->_state_content._state = json_light::StateNumeric;//the only possible case
				break;
			default:
				this->_parse_offset--;
				if (is_digit()) {
					this->_state_content._state = json_light::StateNumeric;//the only possible case
				} else if (this->_json_string == "null"){
					this->_parse_offset += 4;// "null" len
					this->skip_whitespace();
					if (this->_parse_offset == this->_json_string.size()) {
						this->_state_content._p_element = new json_simple(json_simple::SimpleNull);
						this->_state_content._state = json_light::StateHappyend;
					} else {
						throw json_light_exception("json_light::parse_initial(). end of line expected");
					}
				} else if (this->_json_string == "true") {
					this->_parse_offset += 4;// "true" len
					this->skip_whitespace();
					if (this->_parse_offset == this->_json_string.size()) {
						this->_state_content._p_element = new json_simple(json_simple::SimpleTrue);
						this->_state_content._state = json_light::StateHappyend;
					} else {
						throw json_light_exception("json_light::parse_initial(). end of line expected");
					}
				} else if (this->_json_string == "false") {
					this->_parse_offset += 5;// "false" len
					this->skip_whitespace();
					if (this->_parse_offset == this->_json_string.size()) {
						this->_state_content._p_element = new json_simple(json_simple::SimpleFalse);
						this->_state_content._state = json_light::StateHappyend;
					} else {
						this->_state_content._state = json_light::StateFailure;
						throw json_light_exception("json_light::parse_initial(). end of line expected");
					}
				} else {
					throw json_light_exception("json_light::parse_initial() unexpected content");
				}
				break;
			}
		} else {
			throw json_light_exception("json_light::parse_initial(). one of the following expected: Expecting 'STRING', 'NUMBER', 'NULL', 'TRUE', 'FALSE', '{', '['");
		}
	} else {
		throw json_light_exception("call to json_light::parse_initial() on level not 0");
	}
}
void json_light::parse_object() {
	this->skip_whitespace();
	char ch = this->_json_string.at(this->_parse_offset);
	switch (ch){
	case '}':
		this->_parse_offset++;
		if (this->_content_stack.size() == 0) {
			this->skip_whitespace();
			if (this->_parse_offset == this->_json_string.size()) {
				this->_state_content._state = json_light::StateHappyend;
			} else {
				this->_state_content._state = json_light::StateFailure;
				throw json_light_exception("json_light::parse_object() some nonwhite characters after trailing }");
			}
		} else {
			StateContent parent_content = this->_content_stack.back();
			this->_content_stack.pop_back();
			if (parent_content._p_element != NULL) {
				switch (parent_content._p_element->get_type()) {
				case json_element::TypeArray: {
					json_array* parr = dynamic_cast<json_array*>(parent_content._p_element);
					if (parr != NULL) {
						parr->add(this->_state_content._p_element);
						this->_state_content = parent_content;
						this->_state_content._state = json_light::StateArray;
					} else {
						throw json_light_exception("json_light::parse_object() not an array element in parent content");
					}
				}
					break;
				case json_element::TypeObject: {
					json_object* pobj = dynamic_cast<json_object*>(parent_content._p_element);
					if (pobj != NULL) {
						pobj->add(parent_content._key, this->_state_content._p_element);
						parent_content._key.clear();
						this->_state_content = parent_content;
						this->_state_content._state = json_light::StateObject;
					} else {
						throw json_light_exception("json_light::parse_object() not an object element in parent content");
					}
				}
					break;
				default:
					throw json_light_exception("json_light::parse_object() invalid object element in parent content");
				}
			}
#ifdef DEBUG_PARSING
			this->where_i_am();
#endif
		}
		break;
	case '"':
		this->_parse_offset++;
		this->_state_content._state = json_light::StateKVP;
		break;
	case ',':
		this->_parse_offset++;
		break;
	default:
		throw json_light_exception("json_light::parse_object() not { at start");
	}
}
void json_light::parse_array() {
	std::string str;
	json_object* pobj = NULL;
	this->skip_whitespace();
	char ch = this->_json_string.at(this->_parse_offset++);
	switch (ch) {
	case ']':
#ifdef DEBUG_PARSING
		where_i_am();
#endif
		if (this->_content_stack.size() == 0) {
			this->skip_whitespace();
			if (this->_parse_offset == this->_json_string.size()) {
				this->_state_content._state = json_light::StateHappyend;
			} else {
				this->_state_content._state = json_light::StateFailure;
				throw json_light_exception("json_light::parse_array() some nonwhite characters after trailing }");
			}
		} else {
			StateContent parent_content = this->_content_stack.back();
			this->_content_stack.pop_back();
			if (parent_content._p_element != NULL) {
				switch (parent_content._p_element->get_type()) {
				case json_element::TypeArray: {
					json_array* parr = dynamic_cast<json_array*>(parent_content._p_element);
					if (parr != NULL) {
						parr->add(this->_state_content._p_element);
						this->_state_content = parent_content;
						this->_state_content._state = json_light::StateArray;
					} else {
						throw json_light_exception("json_light::parse_array() not an array element in parent content");
					}
				}
											break;
				case json_element::TypeObject: {
					json_object* pobj = dynamic_cast<json_object*>(parent_content._p_element);
					if (pobj != NULL) {
						pobj->add(parent_content._key, this->_state_content._p_element);
						parent_content._key.clear();
						this->_state_content = parent_content;
						this->_state_content._state = json_light::StateObject;
					} else {
						throw json_light_exception("json_light::parse_array() not an object element in parent content");
					}
				}
											 break;
				default:
					throw json_light_exception("json_light::parse_array() invalid object element in parent content");
				}
			}
#ifdef DEBUG_PARSING
			this->where_i_am();
#endif
		}
		break;
	case '{':
		//our current content should has an json_array element
		//we push it into the stack and reinit current to be a new object-value
		this->_content_stack.push_back(this->_state_content);
		this->_state_content._p_element = new json_object();
		this->_state_content._key.clear();
		this->_state_content._state = json_light::StateObject;
		this->_parse_offset++;
		break;
	case '[':
		//our current content should has an json_object element
		//we push it into the stack and reinit current to be a new array-value
		this->_content_stack.push_back(this->_state_content);
		this->_state_content._p_element = new json_array();
		this->_state_content._key.clear();
		this->_state_content._state = json_light::StateArray;
		break;
	case '"':
		str = this->read_string();
		pobj = dynamic_cast<json_object*>(this->_state_content._p_element);
		if (pobj != NULL) {
			pobj->add(this->_state_content._key, &str);
			this->_state_content._state = json_light::StateObject;
		} else {
			throw json_light_exception("json_light::parse_array() not an object element");
		}
		break;
	case '-':
		this->_parse_offset--;//include minus sign
		str = this->read_numeric();
		pobj = dynamic_cast<json_object*>(this->_state_content._p_element);
		if (pobj != NULL) {
			pobj->add_numeric(this->_state_content._key, &str);
			this->_state_content._state = json_light::StateObject;
		} else {
			throw json_light_exception("json_light::parse_array() not an object element");
		}
		break;
	default:
		throw json_light_exception("json_light::parse_array() not { at start");
	}
}
std::string json_light::read_string() {
//read until ", set pstr value, set offset next to " and returns resulting string
//throws exceptioin if " not found
	std::string str;
	size_t pos = this->_json_string.find('\"', this->_parse_offset);
	if (pos != std::string::npos) {
		str.assign(this->_json_string.substr(this->_parse_offset, pos - this->_parse_offset));
		this->_parse_offset = pos + 1;
	} else {
		throw json_light_exception("json_light::read_string() end of string not found");;
	}
	return str;
}
std::string json_light::read_numeric() {
	//read until oneof the following found: digit, whitespace, '}', end of line, set str value, set offset next to find character and returns the str
	//throws exceptioin if " not found
#ifdef DEBUG_PARSING
	this->where_i_am();
#endif
	std::string str;
	size_t pos = this->_json_string.find_first_not_of(_numerics, this->_parse_offset);
	if (pos != std::string::npos) {
		str.assign(this->_json_string.substr(this->_parse_offset, pos - this->_parse_offset));
		this->_parse_offset = pos + 1;
		if (!json_number::is_string_numeric(&str)) {
			throw json_light_exception("json_light::read_numeric() the string is not numeric");
		}
	} else {
		throw json_light_exception("json_light::read_numeric() end of numeric not found");
	}
	return str;
}
void json_light::parse_kvp() {
	std::string str;
	json_object* pobj = NULL;
	size_t pos = this->_json_string.find('\"', this->_parse_offset);
	if (pos != std::string::npos) {
		str = this->read_string();
		this->_state_content._key = str;
		skip_colon();
		char ch = this->_json_string.at(this->_parse_offset++);
		switch (ch) {
		case '{':
			//our current content should has an json_object element
			//we push it into the stack and reinit current to be a new object-value
			this->_content_stack.push_back(this->_state_content);
			this->_state_content._p_element = new json_object();
			this->_state_content._key.clear();
			this->_state_content._state = json_light::StateObject;
			this->_parse_offset++;
			break;
		case '[':
			//our current content should has an json_object element
			//we push it into the stack and reinit current to be a new array-value
			this->_content_stack.push_back(this->_state_content);
			this->_state_content._p_element = new json_array();
			this->_state_content._key.clear();
			this->_state_content._state = json_light::StateArray;
			break;
		case '"':
			str = this->read_string();
			pobj = dynamic_cast<json_object*>(this->_state_content._p_element);
			if (pobj != NULL) {
				pobj->add(this->_state_content._key, &str);
				this->_state_content._state = json_light::StateObject;
			} else {
				throw json_light_exception("json_light::parse_kvp() not an object element");
			}
			break;
		case '-':
			this->_parse_offset--;//include minus sign
			str = this->read_numeric();
			pobj = dynamic_cast<json_object*>(this->_state_content._p_element);
			if (pobj != NULL) {
				pobj->add_numeric(this->_state_content._key, &str);
				this->_state_content._state = json_light::StateObject;
			} else {
				throw json_light_exception("json_light::parse_kvp() not an object element");
			}
			break;
		default:
			if (this->is_digit(ch)){
				this->_parse_offset--;//include minus sign
				str = this->read_numeric();
				json_object* pobj = dynamic_cast<json_object*>(this->_state_content._p_element);
				if (pobj != NULL) {
					pobj->add_numeric(this->_state_content._key, &str);
					this->_state_content._state = json_light::StateObject;
				} else {
					throw json_light_exception("json_light::parse_kvp() not an object element");
				}
#ifdef DEBUG_PARSING
				where_i_am();
#endif
			} else {
				throw json_light_exception("json_light::parse_kvp() not { at start");
			}
		}
	} else {
		throw json_light_exception("json_light::parse_kvp() end of key not found");
	}
}
void json_light::parse_key() {
	size_t pos = this->_json_string.find('\"', this->_parse_offset);
	if (pos != std::string::npos) {
		this->_state_content._key = this->_json_string.substr(this->_parse_offset, pos - this->_parse_offset);
		this->_parse_offset = pos + 1;
		this->skip_colon();
		this->_state_content._state = json_light::StateValue;
	} else {
		throw json_light_exception("json_light::parse_key() end of key not found");
	}
}
void json_light::parse_numeric() {
	size_t pos = this->_json_string.find_first_not_of(json_light::_numerics, this->_parse_offset);
	if (pos != std::string::npos) {
		std::string numstr = this->_json_string.substr(this->_parse_offset, pos - this->_parse_offset);
		if (json_number::is_string_numeric(&numstr)) {
			this->_parse_offset = pos + 1;
			json_light::StateContent parent_content = this->_content_stack.back();
			this->_content_stack.pop_back();
			this->_parse_offset = pos + 1;
			//add numeric element to parent
			switch (parent_content._p_element->get_type()) {
			case json_element::TypeObject: {
				json_object* p = dynamic_cast<json_object*>(parent_content._p_element);
				p->add(this->_state_content._key, new json_number(&numstr));
				this->_state_content = parent_content;
			}
				break;
			default:
				throw json_light_exception("json_light::parse_numeric() todo");
			}
		} else {
			throw json_light_exception("json_light::parse_numeric() valid number expected");
		}
	} else {
		throw json_light_exception("json_light::parse_numeric() end of key not found");
	}
}
void json_light::parse_string() {
	size_t pos = this->_json_string.find('\"', this->_parse_offset);
	if (pos != std::string::npos) {
		std::string value_str = this->_json_string.substr(this->_parse_offset, pos - this->_parse_offset);
		json_light::StateContent parent_content = this->_content_stack.back();
		this->_content_stack.pop_back();
		this->_parse_offset = pos+1;
		//add string element to parent
		switch (parent_content._p_element->get_type()) {
		case json_element::TypeObject: {
			json_object* p = dynamic_cast<json_object*>(parent_content._p_element);
			p->add(this->_state_content._key, new json_string(value_str));
			this->_state_content = parent_content;
		}
			break;
		default:
			throw json_light_exception("json_light::parse_numeric() todo");
		}

	} else {
		throw json_light_exception("json_light::parse_numeric() end of key not found");
	}
}
void json_light::parse_value() {
	this->skip_whitespace();
	char ch = this->_json_string.at(this->_parse_offset);
	switch (ch) {
	case '{':
		this->_state_content._state = json_light::StateObject;
		this->_parse_offset++;
		break;
	case '[':
		this->_state_content._state = json_light::StateArray;
		this->_parse_offset++;
		break;
	case '"':
		this->_parse_offset++;
		this->_content_stack.push_back(this->_state_content);
		this->_state_content._state = json_light::StateString;
		break;
	case '-':
		this->_state_content._state = json_light::StateNumeric;
		break;
	default:
		throw json_light_exception("json_light::parse_value() not { at start");
	}
}
std::string json_light::state2str(State state) {
	std::string str;
	switch (state) {
	case StateInitial: str = "StateInitial";	break;
	case StateObject: str = "StateObject";	break;
	case StateArray: str = "StateArray";	break;
	case StateNumeric: str = "StateNumeric";	break;
	case StateString: str = "StateString";	break;
	case StateKVP: str = "StateKVP";	break;
	case StateKey: str = "StateKey";	break;
	case StateValue: str = "StateValue";	break;
	case StateHappyend: str = "StateHappyend";	break;
	case StateFailure: str = "StateFailure";	break;
	default:
		throw 0;
	}
	return str;
}
#ifdef DEBUG_PARSING
void json_light::where_i_am() {
	int sz = (int)this->_content_stack.size();
	char pointer_ch = '_';
	std::cout << "state: " << this->state2str(this->_state_content._state).c_str() << ", stack size: " << sz << std::endl;
	if (this->_parse_offset == 0) {
		std::cout << pointer_ch << this->_json_string.c_str() << std::endl;
	} else if (this->_parse_offset < this->_json_string.size()) {
		std::cout << this->_json_string.substr(0, this->_parse_offset).c_str() << pointer_ch << this->_json_string.substr(this->_parse_offset).c_str() << std::endl;
	} else {
		std::cout << this->_json_string.c_str() << pointer_ch << std::endl;
	}
}
#endif
void json_light::parse(const char* pjsonstr) {
	this->clear();
	this->_json_string = pjsonstr;
	while (this->_state_content._state != StateHappyend && this->_state_content._state != StateFailure) {
#ifdef DEBUG_PARSING
		where_i_am();
#endif
		switch (this->_state_content._state) {
		case StateInitial:
			this->parse_initial();
			break;
		case StateObject:
			this->parse_object();
			break;
		case StateArray:
			this->parse_array();
			break;
		case StateNumeric:
			this->parse_numeric();
			break;
		case StateString:
			this->parse_string();
			break;
		case StateKVP:
			this->parse_kvp();
			break;
		case StateKey:
			this->parse_key();
			break;
		case StateValue:
			this->parse_value();
			break;
		case StateHappyend:
			this->parse_array();
			break;
		case StateFailure:
			this->parse_array();
			break;
		default:
			throw json_light_exception("json_light::parse() invalie State");
		}
	}
#ifdef DEBUG_PARSING
	where_i_am();
#endif
}
int json_light::get_int(std::string path_str) {
	int iresult = 0;
	if (this->_state_content._state == StateHappyend) {
		std::vector<std::string> strings;
		std::istringstream f(path_str);
		std::string s;
		json_element *p_root_element = this->_state_content._p_element;
		json_element *p_current_element = NULL;
		json_object *p_object = NULL;
		json_number *p_number = NULL;
		json_string *p_string = NULL;
		json_element::Type type = p_root_element->get_type();
		bool ok = p_root_element != NULL;
		ok = p_root_element->get_type() == json_element::TypeObject;
		if (p_root_element != NULL && p_root_element->get_type() == json_element::TypeObject) {
			p_current_element = p_root_element;
			while (std::getline(f, s, '.')) {
				switch (p_current_element->get_type()) {
				case json_element::TypeObject:
					p_object = dynamic_cast<json_object*>(p_current_element);
					p_current_element = p_object->get_child(s);
					break;
				}
#ifdef DEBUG_PARSING
				std::cout << s << std::endl;
#endif
				strings.push_back(s);
			}
			if (p_current_element != NULL && p_current_element->get_type() == json_element::TypeNumber) {
				p_number = dynamic_cast<json_number*>(p_current_element);
				iresult = p_number->get_int();
			} else {
				throw json_light_exception("json_light::get_int() no element of type TypeNumber found");
			}
		}
	}
	return iresult;
}
std::string json_light::get_string(std::string path_str){
	std::string str;
	if (this->_state_content._state == StateHappyend) {
		std::vector<std::string> strings;
		std::istringstream f(path_str);
		std::string s;
		json_element* p_root_element = this->_state_content._p_element;
		json_element* p_current_element = NULL;
		json_object* p_object = NULL;
		json_number* p_number = NULL;
		json_string* p_string = NULL;
		json_element::Type type = p_root_element->get_type();
		bool ok = p_root_element != NULL;
		ok = p_root_element->get_type() == json_element::TypeObject;
		if (p_root_element != NULL && p_root_element->get_type() == json_element::TypeObject) {
			p_current_element = p_root_element;
			while (std::getline(f, s, '.')) {
				switch (p_current_element->get_type()) {
				case json_element::TypeObject:
					p_object = dynamic_cast<json_object*>(p_current_element);
					p_current_element = p_object->get_child(s);
					break;
				}
#ifdef DEBUG_PARSING
				std::cout << s << std::endl;
#endif
				strings.push_back(s);
			}
			if (p_current_element != NULL && p_current_element->get_type() == json_element::TypeString) {
				p_string = dynamic_cast<json_string*>(p_current_element);
				str = p_string->get_string();
			}
		}
	}
	return str;
}
json_element::Type json_light::get_root_type() const {
	return this->_state_content._p_element == NULL ? json_element::TypeUndefined : this->_state_content._p_element->get_type();
}
const json_element& json_light::get_root_ref() const {
	if (this->_state_content._p_element != NULL) {
		return *this->_state_content._p_element;
	} else {
		throw json_light_exception("json_light::get_root_type() no root element");
	}
}
const json_object& json_light::obj(const char* pkey_str) const {
	const json_element& root = this->get_root_ref();
	return root.get_object(pkey_str);
}
const json_array& json_light::arr(const char* pkey_str) const {
	const json_element& root = this->get_root_ref();
	return root.get_array(pkey_str);
}


