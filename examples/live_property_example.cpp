#include <iostream>
#include <vector>
#include "live_property.hpp"

int main()
{
    using live_int = nstd::live_property<int>;
    using live_string = nstd::live_property<std::string>;

	live_int int_prop{ "integer property for tests" }, dummy_int_prop{ "dummy" };
	std::vector<nstd::signalslot::connection> conections;
    auto changing_callback = [](auto &&ctx)
    {
        std::cout << "The property '" << ctx.property.name() << "' changing: from [" << ctx.property.value() << "] to [" << ctx.new_value << "]" << std::endl;

        using value_type = typename std::decay<decltype(ctx.property.value())>::type;

        if constexpr (std::is_arithmetic<value_type>::value)
        {
            if (ctx.cancel = ctx.new_value < 0; ctx.cancel)
            {
                std::cout << "<<<negative numbers are not allowed! The change was cancelled by a slot!>>>" << std::endl;
            }
        }
        else if constexpr (std::is_same<value_type, std::string>::value)
        {
            if (ctx.cancel = std::empty(ctx.new_value); ctx.cancel)
            {
                std::cout << "<<<empty strings are not allowed! The change was cancelled by a slot!>>>" << std::endl;
            }
        }
    };
	auto changed_callback = [](auto &&property)
    {
        std::cout << "The property '" << property.name() << "' changed to: " << property.value() << std::endl;
    };

	conections.emplace_back(int_prop.value_changing.connect(changing_callback));
	conections.emplace_back(int_prop.value_changed.connect(changed_callback));

	int raw_int = 50;
	dummy_int_prop = 222;
	int_prop = 1;
	int_prop = 150;
	int_prop = raw_int;
	int_prop *= 7;

	std::cout << "...checking for oprator== works in C++17 as expected..." << std::endl;
	std::cout << "comparing int_prop == dummy_int_prop (expecting: false): " << std::boolalpha << (int_prop == dummy_int_prop) << std::endl;

	int_prop = dummy_int_prop;

	std::cout << "now comparing int_prop == dummy_int_prop (expecting: true): " << (int_prop == dummy_int_prop) << std::endl;

	int_prop = -1;
	std::cout << "int_prop = " << int_prop << std::endl;

	conections.clear(); //auto-disconnect from all slots
    std::cout << "no slots are called from now on since we destroied all connections..." << std::endl << "...setting int_prop to -1 should not be restricted now..." << std::endl;

    int_prop = -1;

    std::cout << "int_prop = " << int_prop << std::endl;

    live_string str_prop{ "string property for tests", "___" }, dummy_string_prop{ "dummy" };

	conections.emplace_back(str_prop.value_changing.connect(changing_callback));
	conections.emplace_back(str_prop.value_changed.connect(changed_callback));

	str_prop = "Hello World!";
	str_prop = "";

	std::cout << "str_prop = " << str_prop.value() << std::endl;

    return 0;
}
