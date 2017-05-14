#include <iostream>
#include <string>
#include "expiry_cache.hpp"

using namespace std::string_literals;

struct Item
{
	Item() = default;
	~Item() { std::cout << "Item deleted..." << std::endl; }
};

int main()
{
	using namespace std::chrono_literals;

	nstd::expiry_cache<std::string, Item*> a(800ms);
	a.set_vacuum_idle_period(200ms);
	auto c = a.expired.connect([](auto &k, auto &v)
    {
        std::cout << "Key: '" << k << "' expired: " << v << std::endl;
        delete v;
    });

	a.start_auto_vacuum();

	const auto key { "My item"s };
	a.put(key, new Item);

	if (Item* b = nullptr; a.get(key, b))
    {
        std::cout << "Item '" << key << "' is found! :) address: " << b << std::endl;
    }
    else
    {
        std::cout << "Item is not found :(" << std::endl;
    }

	std::cout << "Sleeping for 150 ms..." << std::endl;
	std::this_thread::sleep_for(150ms);

	std::cout << "Container size: " << a.size() << std::endl;

	std::cout << "Sleeping for 1 sec..." << std::endl;
	std::this_thread::sleep_for(1s);

	std::cout << "Container size: " << a.size() << std::endl;

    return 0;
}
