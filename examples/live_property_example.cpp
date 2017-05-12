#include <iostream>
#include <vector>
#include "live_property.hpp"

int main()
{
	nstd::live_property<int> myInt{ "Test int" }, myInt2{ "!!!" };
	std::vector<nstd::signalslot::connection> cons;
	cons.emplace_back(
    myInt.value_changing.connect([](auto &&prop, auto &&ctx)
    {
        std::cout << "The property '" << prop.name() << "' changing: from [" << prop << "] to [" << ctx.new_value << "]" << std::endl;

        if (ctx.cancel = ctx.new_value < 0; ctx.cancel)
        {
            std::cout << "<<<negative integers are not allowed! The change was cancelled!>>>" << std::endl;
        }
    })
    );
	cons.emplace_back(
    myInt.value_changed.connect([](auto &&prop)
    {
        std::cout << "The property '" << prop.name() << "' changed to: " << prop << std::endl;
    })
    );

	int t = 50;
	myInt2 = 222;
	myInt = 1;
	myInt = 150;
	myInt = t;
	myInt = myInt2;
	myInt = -1;
	std::cout << myInt << std::endl;

	return 0;
}
