//SPDX-FileCopyrightText: © 2024 Anatolij Vasylchenko <anatolij_vasilcenko@gmx.net>
//SPDX-License-Identifier: MIT

#pragma once
#include "json_element.h"
class json_number : public json_element {
public:
	json_number();
	json_number(const std::string *pstr);
	int get_int() const;
	static bool is_string_numeric(const std::string *pstr);
	static bool is_string_numeric(const char *pstr);
private:
	std::string _numeric_string;
	enum State { StateInitial, StateAfterSign, StateInDigits, StateScientific, StateHappyend, StateFailure };

	static State is_string_numeric_initial(const char *p_str, size_t *p_offset, bool *p_met_decimal_point);
	static State is_string_numeric_aftersign(const char *pstr, size_t* p_offset, bool* p_met_decimal_point);
	static State is_string_numeric_indigits(const char *pstr, size_t* p_offset, bool* p_met_decimal_point, bool* p_scientific);
	static State is_string_numeric_scientific(const char *pstr, size_t* p_offset, bool* p_scientific_sign, bool* p_scientific_digit);


	static State is_string_numeric_initial(const std::string *pstr, size_t* p_offset, bool *p_met_decimal_point);
	static State is_string_numeric_aftersign(const std::string *pstr, size_t* p_offset, bool *p_met_decimal_point);
	static State is_string_numeric_indigits(const std::string *pstr, size_t* p_offset, bool *p_met_decimal_point, bool *p_scientific);
	static State is_string_numeric_scientific(const std::string *pstr, size_t* p_offset, bool *p_scientific_sign, bool *p_scientific_digit);
};

