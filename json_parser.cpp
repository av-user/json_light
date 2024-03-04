#include <iostream>
#include <sstream>
#include "json_number.h"
#include "json_array.h"

#include "json_light.h"

int main() {
    std::cout << "Hello World!\n";

    std::string jsonstr3 {
        R"(
            {
              "name": "Roderick",
              "role": "Barbarian",
              "guild": {
                "name": "The Bandits",
                "":[]
              },
              "equipment": [{
                "name": "Iron Sword",
                "damage": 5
              }]
            }
        )"
    };

    json_light jl;

    try {
        std::cout << "jsonstr3: " << jsonstr3 << std::endl;
        jl.parse(jsonstr3.c_str());
        json_element::Type type = jl.get_root_type();
        if (type == json_element::TypeObject) {
            const std::string s = jl.get_root_ref().get_string("role");
            std::cout << s.c_str() << std::endl;
            int i = jl.get_root_ref().get_array("equipment").at(0).get_int("damage");
            std::cout << "i: " << i << std::endl;
        }
        int integer;
        std::string str = jl.get_string("role");
        std::cout << "role: " << str.c_str() << std::endl;
        str = jl.obj("guild").get_string("name");
        std::cout << "name: " << str.c_str() << std::endl;
        integer = jl.get_int("key");
        integer = jl.get_int("key_num");
        std::cout << integer << std::endl;
    } catch (json_light_exception& ex) {
        std::cout << "json_light_exception message: " << ex.message().c_str() << std::endl;
    }
    const char  *jsonstr = "   \t{ \"key\": \"value\" \n }    \t  ";
    std::cout << "jsonstr: " << jsonstr << std::endl;
    try {
        jl.parse(jsonstr);
        std::string str = jl.get_string("key");
        std::cout << "str: " << str.c_str() << std::endl;
    } catch (json_light_exception& ex) {
        std::cout << "json_light_exception message: " << ex.message().c_str() << std::endl;
    }
    const char* jsonstr2 = "   \t{ \"key\": \"value\" \n, \"key_num\" : -1234e5 }    \t  ";
    try{
        jl.parse(jsonstr2);
        int integer = jl.get_int("key");
        integer = jl.get_int("key_num");
        std::cout << integer << std::endl;
    } catch (json_light_exception& ex) {
        std::cout << "json_light_exception message: " << ex.message().c_str() << std::endl;
    }

}
