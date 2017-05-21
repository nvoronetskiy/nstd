#pragma once

/*
MIT License
Copyright (c) 2017 Arlen Keshabyan (arlen.albert@gmail.com)
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>

namespace nstd
{
using namespace std::chrono_literals;
namespace signal_slot
{
template <typename T>
struct has_value_type
{
    T value = {};

    has_value_type() = default;
    has_value_type(has_value_type &&) = default;
    has_value_type &operator= (has_value_type &&) = default;
};

struct no_value_type
{
    no_value_type() = default;
    no_value_type(no_value_type &&) = default;
    no_value_type &operator= (no_value_type &&) = default;
};

template <typename T1, typename T2>
using base_value_type = typename std::conditional<std::is_same<T1, std::void_t<>>::value, no_value_type, has_value_type<T1>>::type;

template<typename T1 = std::void_t<>, typename T2 = std::void_t<>>
class paired_ptr : public base_value_type<T1, T2>
{
public:
	using base_type = base_value_type<T1, T2>;
	using connected_paired_ptr_type = paired_ptr<T2, T1>;

	template<typename, typename> friend struct paired_ptr;
	template<typename T12, typename T22> friend bool operator==(const paired_ptr<T12, T22> &, const paired_ptr<T12, T22> &);
	template<typename T12, typename T22> friend bool operator!=(const paired_ptr<T12, T22> &, const paired_ptr<T12, T22> &);
	template<typename T12, typename T22> friend bool operator==(const paired_ptr<T12, T22> &, std::nullptr_t);
	template<typename T12, typename T22> friend bool operator!=(const paired_ptr<T12, T22> &, std::nullptr_t);
	template<typename T12, typename T22> friend bool operator==(std::nullptr_t, const paired_ptr<T12, T22> &);
	template<typename T12, typename T22> friend bool operator!=(std::nullptr_t, const paired_ptr<T12, T22> &);

	paired_ptr()
	{
	}

	paired_ptr(connected_paired_ptr_type *ptr) : _connected_paired_ptr{ ptr }
	{
		if (_connected_paired_ptr) _connected_paired_ptr->_connected_paired_ptr = this;
	}

	paired_ptr(paired_ptr &&other) : paired_ptr{ other._connected_paired_ptr }
	{
		base_type::operator=(static_cast<base_type&&>(std::forward<paired_ptr>(other)));
		other._connected_paired_ptr = nullptr;
	}

	paired_ptr & operator=(paired_ptr &&other)
	{
		if (_connected_paired_ptr)
		{
			if (other._connected_paired_ptr)
				std::swap(_connected_paired_ptr->_connected_paired_ptr, other._connected_paired_ptr->_connected_paired_ptr);
			else
				_connected_paired_ptr->_connected_paired_ptr = &other;
		}
		else if (other._connected_paired_ptr)
			other._connected_paired_ptr->_connected_paired_ptr = this;

		std::swap(_connected_paired_ptr, other._connected_paired_ptr);

		base_type::operator=(static_cast<base_type&&>(std::forward<paired_ptr>(other)));

		return *this;
	}

	paired_ptr & operator=(connected_paired_ptr_type *ptr)
	{
		return *this = paired_ptr(ptr);
	}

	~paired_ptr()
	{
		disconnect();
	}

	void disconnect()
	{
		if (_connected_paired_ptr)
		{
			_connected_paired_ptr->_connected_paired_ptr = nullptr;
			_connected_paired_ptr = nullptr;
		}
	}

	const connected_paired_ptr_type * connected_ptr() const
	{
		return _connected_paired_ptr;
	}

	operator bool() const
	{
        	return (_connected_paired_ptr);
	}

	bool operator!() const
	{
        	return !(_connected_paired_ptr);
	}

protected:
	connected_paired_ptr_type *_connected_paired_ptr = nullptr;

};

template<typename T1, typename T2>
bool operator==(const paired_ptr<T1, T2> & lhs, const paired_ptr<T1, T2> & rhs)
{
    return lhs._connected_paired_ptr == rhs._connected_paired_ptr;
}
template<typename T1, typename T2>
bool operator!=(const paired_ptr<T1, T2> & lhs, const paired_ptr<T1, T2> & rhs)
{
    return !(lhs == rhs);
}
template<typename T1, typename T2>
bool operator==(const paired_ptr<T1, T2> & lhs, std::nullptr_t)
{
    return !lhs;
}
template<typename T1, typename T2>
bool operator!=(const paired_ptr<T1, T2> & lhs, std::nullptr_t)
{
    return !(lhs == nullptr);
}
template<typename T1, typename T2>
bool operator==(std::nullptr_t, const paired_ptr<T1, T2> & rhs)
{
    return rhs == nullptr;
}
template<typename T1, typename T2>
bool operator!=(std::nullptr_t, const paired_ptr<T1, T2> & rhs)
{
    return !(nullptr == rhs);
}

template<typename... Args>
class slot
{
protected:
    std::function<void (Args...)> _functor;
    paired_ptr<> _connection;

    friend class connection;

public:
    slot() = default;
    slot(std::function<void(Args...)> &&f) : _functor{ std::forward<std::function<void(Args...)>>(f) }{}
    void operator()(const Args &... args) { _functor(args...); }
    bool is_disconnected() const { return !_connection; }
    void clear() { _connection.disconnect(); }
};

class signal_base
{
public:
    virtual ~signal_base() = default;
    virtual std::string_view name() const = 0;
    virtual bool enabled() const = 0;
    virtual void enabled(bool enabled) const = 0;
};

class connection
{
public:
    connection() = default;

    template<typename... Args>
    connection(const signal_base *signal, slot<Args...> &s) : _slot{ &s._connection }, _signal { signal }
    {
    }

    connection(connection &&other) = default;
    connection &operator=(connection &&other)
    {
        disconnect();

        _slot = std::move(other._slot);
        _signal = std::move(other._signal);

        return *this;
    }

    ~connection()
    {
        disconnect();
    }

    void disconnect()
    {
        if (_slot) _slot.disconnect();

        _signal = nullptr;
    }

    bool is_disconnected() const { return !_slot; }

    const signal_base &signal() const
    {
        return *_signal;
    }

protected:
    paired_ptr<> _slot;
    const signal_base *_signal = nullptr;
};

template<typename... Args>
class signal : public signal_base
{
public:
    signal() = default;
    signal(const std::string &name) : _name{ name } {}
    signal(signal &&other) = default;
    signal &operator=(signal &&other) = default;
    virtual ~signal() override = default;

    void emit(const Args &... args)
    {
        if (!_enabled) return;

        std::scoped_lock lock(_emit_lock);

        if (!_enabled) return;

        {
            std::scoped_lock lock(_connect_lock);

            std::move(_pending_connections.begin(), _pending_connections.end(), std::back_inserter(_slots));

            _pending_connections.clear();
        }

        auto end = _slots.end();

        _slots.erase(std::remove_if(_slots.begin(), end, [&args...](auto &callable)
        {
            if (callable.is_disconnected()) return true;

            callable(args...);

            return callable.is_disconnected();
        }), end);
    }

    connection connect(std::function<void(Args...)> &&callable)
    {
        slot<Args...> new_slot(std::forward<std::function<void(Args...)>>(callable));
        connection to_return(this, new_slot);

        std::scoped_lock lock(_connect_lock);

        _pending_connections.erase(std::remove_if(_pending_connections.begin(), _pending_connections.end(), std::mem_fn(&slot<Args...>::is_disconnected)), _pending_connections.end());
        _pending_connections.emplace_back(std::move(new_slot));

        return to_return;
    }

    template<typename T>
    connection connect(T *instance, void (T::*member_function)(Args...))
    {
        return connect([instance, member_function](Args... args) { (instance->*member_function)(args...); });
    }

    void clear()
    {
        std::scoped_lock lock(_connect_lock, _emit_lock);

        _pending_connections.clear();
        _slots.clear();
    }

    size_t size() const
    {
        std::scoped_lock lock(_emit_lock);

        return _slots.size();
    }

    void name(const std::string &name)
    {
        std::scoped_lock lock(_name_lock);

        _name = name;
    }

    virtual std::string_view name() const override
    {
        std::scoped_lock lock(_name_lock);

        return _name;
    }

    virtual bool enabled() const
    {
        return _enabled;
    }

    virtual void enabled(bool enabled) const
    {
        _enabled = enabled;
    }

protected:
    std::string _name;
    std::vector<slot<Args...>> _slots, _pending_connections;
    mutable std::mutex _connect_lock, _emit_lock, _name_lock;
    mutable std::atomic_bool _enabled = true;
};

template<typename... Args>
class throttled_signal : public signal<Args...>
{
public:
    using base_class = signal<Args...>;

    throttled_signal() = default;
    throttled_signal(const std::string &name, const std::chrono::milliseconds &throttle_ms = 100ms) : base_class{ name }, _throttle_ms{ throttle_ms } {}
    throttled_signal(throttled_signal &&other) = default;
    throttled_signal &operator=(throttled_signal &&other) = default;

    virtual ~throttled_signal() override
    {
        _cancelled = true;

        if (_dispatcher_thread.joinable()) _dispatcher_thread.join();

        std::scoped_lock lock(_emit_lock);

        while (!std::empty(_signal_queue))
        {
            auto args = std::move(_signal_queue.front());
            _signal_queue.pop();

            std::apply([&](const Args&... a){ base_class::emit(a...); }, args);
        }
    }

    void emit(const Args &... args)
    {
        std::scoped_lock lock(_emit_lock);

        _signal_queue.push(std::make_tuple(args...));

        if (!_thread_running)
        {
            _thread_running = true;
            _dispatcher_thread = std::thread([this](){ queue_dispatcher(); });
        }
    }

    void throttle_ms(const std::chrono::milliseconds &duration)
    {
        _throttle_ms = duration;
    }

    std::chrono::microseconds throttle_ms() const
    {
        return _throttle_ms;
    }

protected:
    std::queue<std::tuple<Args...>> _signal_queue;
    std::atomic_bool _cancelled { false };
    std::mutex _emit_lock;
    std::atomic<std::chrono::milliseconds> _throttle_ms { 100ms };
    std::thread _dispatcher_thread;
    std::atomic_bool _thread_running { false };

    void queue_dispatcher()
    {
        while (!_cancelled && !std::empty(_signal_queue))
        {
            {
                std::unique_lock lock(_emit_lock);
                auto args = std::move(_signal_queue.front());

                _signal_queue.pop();

                std::apply([&](const Args&... a){ base_class::emit(a...); }, args);
            }

            std::this_thread::sleep_for(_throttle_ms.load());
        }

        _thread_running = false;
    }
};

template<typename... Args>
class timer_signal : public signal<timer_signal<Args...>*, Args...>
{
public:
    using base_class = signal<timer_signal*, Args...>;

    timer_signal() = default;
    timer_signal(const std::string &name, const std::chrono::milliseconds &timer_ms = 1s) : base_class{ name }, _timer_ms{ timer_ms } {}
    timer_signal(timer_signal &&other) = default;
    timer_signal &operator=(timer_signal &&other) = default;

    virtual ~timer_signal() override
    {
        stop_timer();
    }

    void start_timer(const Args&... args)
    {
        _args = std::make_tuple(this, args...);

        _enabled = true;

        _timer_thread = std::thread([this](){ timer_procedure(); });
    }

    void stop_timer()
    {
        _enabled = false;

        if (_timer_thread.joinable()) _timer_thread.join();
    }

    void disable_timer_from_slot()
    {
        _enabled = false;
    }

    void timer_ms(const std::chrono::milliseconds &duration)
    {
        _timer_ms = duration;
    }

    std::chrono::microseconds timer_ms() const
    {
        return _timer_ms;
    }

private:
    std::atomic_bool _enabled { false };
    std::thread _timer_thread;
    std::atomic<std::chrono::milliseconds> _timer_ms { 1s };
    mutable std::mutex _emit_lock;
    std::tuple<timer_signal*, Args...> _args;

    void timer_procedure()
    {
        while (_enabled)
        {
            {
                std::unique_lock lock(_emit_lock);

                std::apply([&](timer_signal *s, const Args&... a){ base_class::emit(s, a...); }, _args);

                if (!_enabled) return;
            }

            std::this_thread::sleep_for(_timer_ms.load());
        }

        _enabled = false;
    }
};

}
}
