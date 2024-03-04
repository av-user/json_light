//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#include "json_number.h"
json_number::json_number() : json_element (json_element::TypeNumber)
{}
json_number::json_number(const std::string *pstr) 
: json_element(json_element::TypeNumber)
, _numeric_string(*pstr)
{
}
int json_number::get_int() const{
	return ::atoi(this->_numeric_string.c_str());
}
bool is_digit(char ch) { return ch >= '0' && ch <= '9'; }
json_number::State json_number::is_string_numeric_initial(const char *p_str, size_t* p_offset, bool* p_met_decimal_point) {
	json_number::State state = StateFailure;
	size_t len = ::strlen(p_str);
	if (len == 0) {
		state = StateFailure;
	} else {
		char ch = *(p_str +((*p_offset)++));
		switch (ch) {
		case '-':
			state = StateAfterSign;
			break;
		case '.':
			*p_met_decimal_point = true;
			state = StateInDigits;
			break;
		default:
			if (is_digit(ch)) {
				state = StateInDigits;
			} else {
				state = StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_aftersign(const char *pstr, size_t* p_offset, bool* p_met_decimal_point) {
	json_number::State state = StateFailure;
	size_t len = ::strlen(pstr);
	if (*p_offset == len) {
		state = json_number::StateFailure;
	} else {
		char ch = *(pstr + ((*p_offset)++));
		switch (ch) {
		case '.':
			if (*p_met_decimal_point) {
				state = json_number::StateFailure;
			} else {
				*p_met_decimal_point = true;
			}
			break;
		default:
			if (is_digit(ch)) {
				state = json_number::StateInDigits;
			} else {
				state = json_number::StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_indigits(const char *pstr, size_t* p_offset, bool* p_met_decimal_point, bool* p_scientific) {
	State state = StateFailure;
	size_t len = ::strlen(pstr);
	if (*p_offset == len) {
		state = json_number::StateFailure;
	} else {
		char ch = *(pstr + ((*p_offset)++));
		switch (ch) {
		case '.':
			if (*p_met_decimal_point) {
				state = json_number::StateFailure;
			} else {
				*p_met_decimal_point = true;
				state = StateInDigits;
			}
			break;
		case 'e':
		case 'E':
			if (*p_scientific) {
				state = json_number::StateFailure;
			} else {
				state = StateScientific;
				*p_scientific = true;
			}
			break;
		default:
			if (is_digit(ch)) {
				state = StateInDigits;
			} else {
				state = json_number::StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_scientific(const char *pstr, size_t* p_offset, bool* p_scientific_sign, bool* p_scientific_digit) {
	json_number::State state = StateFailure;
	size_t len = ::strlen(pstr);
	if (*p_offset == len) {
		state = p_scientific_digit ? StateHappyend : StateFailure;
	} else {
		char ch = *(pstr + ((*p_offset)++));
		switch (ch) {
		case '-':
			if (*p_scientific_sign) {
				state = json_number::StateFailure;
			} else {
				*p_scientific_sign = true;
				state = json_number::StateScientific;
			}
			break;
		default:
			if (is_digit(ch)) {
				*p_scientific_digit = true;
				state = StateScientific;
			} else {
				state = StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_initial(const std::string *p_str, size_t *p_offset, bool *p_met_decimal_point) {
	json_number::State state = StateFailure;
	if (p_str->empty()) {
		state = StateFailure;
	} else {
		char ch = p_str->at((*p_offset)++);
		switch (ch) {
		case '-':
			state = StateAfterSign;
			break;
		case '.':
			*p_met_decimal_point = true;
			state = StateInDigits;
			break;
		default:
			if (is_digit(ch)) {
				state = StateInDigits;
			} else {
				state = StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_aftersign(const std::string *pstr, size_t *p_offset, bool* p_met_decimal_point) {
	json_number::State state = StateFailure;
	if (*p_offset == pstr->size()) {
		state = json_number::StateFailure;
	} else {
		char ch = pstr->at((*p_offset)++);
		switch (ch) {
		case '.':
			if (*p_met_decimal_point) {
				state = json_number::StateFailure;
			} else {
				*p_met_decimal_point = true;
			}
			break;
		default:
			if (is_digit(ch)) {
				state = json_number::StateInDigits;
			} else {
				state = json_number::StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_indigits(const std::string *pstr, size_t *p_offset, bool* p_met_decimal_point, bool* p_scientific) {
	State state = StateFailure;
	if (*p_offset == pstr->size()) {
		state = *p_offset == 0 ? json_number::StateFailure : json_number::StateHappyend;
	} else {
		char ch = pstr->at((*p_offset)++);
		switch (ch) {
		case '.':
			if (*p_met_decimal_point) {
				state = json_number::StateFailure;
			} else {
				*p_met_decimal_point = true;
				state = StateInDigits;
			}
			break;
		case 'e':
		case 'E':
			if (*p_scientific) {
				state = json_number::StateFailure;
			} else {
				state = StateScientific;
				*p_scientific = true;
			}
			break;
		default:
			if (is_digit(ch)) {
				state = StateInDigits;
			} else {
				state = json_number::StateFailure;
			}
		}
	}
	return state;
}
json_number::State json_number::is_string_numeric_scientific(const std::string *pstr, size_t *p_offset, bool* p_scientific_sign, bool *p_scientific_digit) {
	json_number::State state = StateFailure;
	if (*p_offset == pstr->size()) {
		state = p_scientific_digit ? StateHappyend : StateFailure;
	} else {
		char ch = pstr->at((*p_offset)++);
		switch (ch) {
		case '-':
			if (*p_scientific_sign) {
				state = json_number::StateFailure;
			} else {
				*p_scientific_sign = true;
				state = json_number::StateScientific;
			}
			break;
		default:
			if (is_digit(ch)) {
				*p_scientific_digit = true;
				state = StateScientific;
			} else {
				state = StateFailure;
			}
		}
	}
	return state;
}
bool json_number::is_string_numeric(const char *pstr) {
	size_t offset = 0;
	bool scientific = false;
	bool scientific_sign = false;
	bool scientific_digit = false;
	bool met_decimal_point = false;
	//enum State { Initial, AfterSign, InDigits, Scientific, Happyend, Failure };
	State state = StateInitial;
	while (state != json_number::StateHappyend && state != json_number::StateFailure) {
		switch (state) {
		case json_number::StateInitial:
			state = is_string_numeric_initial(pstr, &offset, &met_decimal_point);
			break;
		case json_number::StateAfterSign:
			state = is_string_numeric_aftersign(pstr, &offset, &met_decimal_point);
			break;
		case json_number::StateInDigits:
			state = is_string_numeric_indigits(pstr, &offset, &met_decimal_point, &scientific);
			break;
		case json_number::StateScientific:
			state = is_string_numeric_scientific(pstr, &offset, &scientific_sign, &scientific_digit);
			break;
		default:
			throw 1;
		}
	}
	return state == StateHappyend;
}
bool json_number::is_string_numeric(const std::string* pstr) {
	size_t offset = 0;
	bool scientific = false;
	bool scientific_sign = false;
	bool scientific_digit = false;
	bool met_decimal_point = false;
	//enum State { Initial, AfterSign, InDigits, Scientific, Happyend, Failure };
	State state = StateInitial;
	while (state != json_number::StateHappyend && state != json_number::StateFailure) {
		switch (state) {
		case json_number::StateInitial:
			state = is_string_numeric_initial(pstr, &offset, &met_decimal_point);
			break;
		case json_number::StateAfterSign:
			state = is_string_numeric_aftersign(pstr, &offset, &met_decimal_point);
			break;
		case json_number::StateInDigits:
			state = is_string_numeric_indigits(pstr, &offset, &met_decimal_point, &scientific);
			break;
		case json_number::StateScientific:
			state = is_string_numeric_scientific(pstr, &offset, &scientific_sign, &scientific_digit);
			break;
		default:
			throw 1;
		}
	}
	return state == StateHappyend;
}