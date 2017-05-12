#include <iostream>
#include <vector>
#include "live_property.hpp"

int main()
{
    using live_int = nstd::live_property<int>;

	live_int int_prop{ "integer property for tests" }, dummy_int_prop{ "dummy" };
	std::vector<nstd::signalslot::connection> conections;
	conections.emplace_back(
    int_prop.value_changing.connect([](auto &&prop, auto &&ctx)
    {
        std::cout << "The property '" << prop.name() << "' changing: from [" << prop << "] to [" << ctx.new_value << "]" << std::endl;

        if (ctx.cancel = ctx.new_value < 0; ctx.cancel)
        {
            std::cout << "<<<negative integers are not allowed! The change was cancelled by a slot!>>>" << std::endl;
        }
    })
    );
	conections.emplace_back(
    int_prop.value_changed.connect([](auto &&prop)
    {
        std::cout << "The property '" << prop.name() << "' changed to: " << prop << std::endl;
    })
    );

	int raw_int = 50;
	dummy_int_prop = 222;
	int_prop = 1;
	int_prop = 150;
	int_prop = raw_int;
	int_prop = dummy_int_prop;
	int_prop = -1;
	std::cout << "int_prop = " << int_prop << std::endl;

	conections.clear(); //auto-disconnect from all slots
    std::cout << "no slots are called from now on since we destroied all connections..." << std::endl << "...setting int_prop to -1 now should work..." << std::endl;

    int_prop = -1;

    std::cout << "int_prop = " << int_prop << std::endl;

    return 0;
}
