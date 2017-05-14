#include <chrono>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include <unordered_map>
#include <tuple>
#include "signal_slot.hpp"

namespace nstd
{
using namespace std::chrono_literals;

template<typename key_type, typename value_type>
class expiry_cache
{
public:
	using self_type = expiry_cache<key_type, value_type>;

	expiry_cache() = default;
	~expiry_cache() { stop_auto_vacuum(); }

	template<typename duration_type>
	expiry_cache(const duration_type &duration)
	{
		set_expiry(duration);
	}

	void put(const key_type &key, const value_type &value, std::chrono::milliseconds expiry_duration_ms = 0ms)
	{
		std::scoped_lock lock {_mutex};

		auto it{ _data.find(key) };

		if (it != _data.end()) _erase(it);

		_data.emplace(key, std::make_tuple(std::chrono::high_resolution_clock::now(), (expiry_duration_ms == 0ms) ? _expiry_duration_ms : expiry_duration_ms, value));
	}

	bool exists(const key_type &key)
	{
		std::scoped_lock lock {_mutex};

		return _data.find(key) != _data.end();
	}

	bool get(const key_type &key, value_type &value)
	{
		std::scoped_lock lock {_mutex};

		auto it{ _data.find(key) };

		if (it != _data.end())
        {
            auto &val = it->second;
            auto now{ std::chrono::high_resolution_clock::now() };

            if (!_auto_vacuum && std::chrono::duration_cast<std::chrono::milliseconds>(now - std::get<0>(val)) > std::get<1>(val))
            {
                _erase(it);

                return false;
            }

            value = std::get<2>(val);

            if (_access_prolongs) std::get<0>(val) = now;

            return true;
        }

        return false;
	}

	void set_access_prolongs(bool prolongs = true)
	{
	    std::scoped_lock lock {_mutex};

		_access_prolongs = prolongs;
	}

	bool is_access_prolongs() const
	{
		return _access_prolongs;
	}

	template<typename duration_type>
	void set_expiry(const duration_type &duration)
	{
	    std::scoped_lock lock {_mutex};

		_expiry_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}

	template<typename duration_type>
	void set_expiry(const key_type &key, const duration_type &duration)
	{
	    std::scoped_lock lock {_mutex};

		auto it{ _data.find(key) };

		if (it != _data.end()) std::get<1>(it->second) = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}

	const std::chrono::milliseconds get_expiry() const
	{
		return _expiry_duration_ms;
	}

	const std::chrono::milliseconds get_expiry(const key_type &key) const
	{
	    std::scoped_lock lock {const_cast<self_type*>(this)->_mutex};

		auto it{ _data.find(key) };

		if (it != _data.end()) return std::get<1>(it->second);

		return 0ms;
	}

	void clear()
	{
		std::scoped_lock lock {_mutex};

		_data.clear();
	}

	size_t size() const
	{
	    std::scoped_lock lock {const_cast<self_type*>(this)->_mutex};

		return _data.size();
	}

	void vacuum()
	{
	    std::scoped_lock lock {_mutex};
		auto now{ std::chrono::high_resolution_clock::now() };
		std::vector<decltype(std::begin(_data))> expired;

		for (auto it{ std::begin(_data) }, end{std::end(_data)}; it != end; ++it)
		{
		    const auto &val = it->second;

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - std::get<0>(val)) > std::get<1>(val))
				expired.emplace_back(it);
		}

		for (auto &it : expired) _erase(it);
	}

	template<typename Duration>
	void set_vacuum_idle_period(const Duration duration)
	{
		_vacuum_idle_period_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	}

	std::chrono::milliseconds get_vacuum_idle_period() const
	{
		return _vacuum_idle_period_ms;
	}

	void start_auto_vacuum()
	{
		if (_auto_vacuum) return;

		std::scoped_lock lock {_mutex};

		if (_auto_vacuum) return;

		_auto_vacuum_thread = std::thread([this]() { _auto_vacuum_procedure(this); });

		_auto_vacuum = true;
	}

	void stop_auto_vacuum()
	{
		if (!_auto_vacuum) return;

		std::scoped_lock lock {_mutex};

		if (!_auto_vacuum) return;

		_cancel_auto_vacuum = true;

		if (_auto_vacuum_thread.joinable()) _auto_vacuum_thread.join();

		_auto_vacuum = _cancel_auto_vacuum = false;
	}

    nstd::signal_slot::signal<const key_type &, value_type &> signal_data_expired;

private:
	std::unordered_map<key_type, std::tuple<std::chrono::time_point<std::chrono::high_resolution_clock>, std::chrono::milliseconds, value_type>> _data;

	void _auto_vacuum_procedure(self_type *this_)
	{
		auto time{ std::chrono::high_resolution_clock::now() };

		while (!this_->_cancel_auto_vacuum)
		{
			auto now{ std::chrono::high_resolution_clock::now() };

			if (std::chrono::duration_cast<std::chrono::milliseconds>(now - time) > _vacuum_idle_period_ms)
			{
				time = now;

				this_->vacuum();
			}

			if (this_->_cancel_auto_vacuum) return;

			std::this_thread::sleep_for(100ms);
		}
	}

	void _erase(decltype(std::begin(_data)) &it)
	{
	    auto &val = it->second;

		signal_data_expired.emit(it->first, std::get<2>(val));

		_data.erase(it);
	}

	std::chrono::milliseconds _expiry_duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(10min);
	std::chrono::milliseconds _vacuum_idle_period_ms = std::chrono::duration_cast<std::chrono::milliseconds>(1min);
	std::mutex _mutex;
	bool _access_prolongs = false;
	bool _auto_vacuum = false;
	std::thread _auto_vacuum_thread;
	volatile bool _cancel_auto_vacuum = false;
};
}
