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

#include <type_traits>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>

namespace nstd
{
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

	explicit operator bool() const
	{
        	return (_connected_paired_ptr);
	}

	bool operator!() const
	{
        	return !(_connected_paired_ptr);
	}

private:
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
private:
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

class connection
{
public:
    connection() = default;

    template<typename... Args>
    explicit connection(slot<Args...> &s, const std::string &connection_name = std::string()) : _slot{ &s._connection }, _connection_name { connection_name }
    {
    }

    connection(connection &&other) = default;
    connection &operator=(connection &&other)
    {
        disconnect();

        _slot = std::move(other._slot);
        _connection_name = std::move(other._connection_name);

        return *this;
    }

    ~connection()
    {
        disconnect();
    }

    void disconnect()
    {
        if (_slot) _slot.disconnect();
    }

    void set_connection_name(const std::string &connection_name)
    {
        _connection_name = connection_name;
    }

    std::string_view get_connection_name() const
    {
        return _connection_name;
    }

protected:
    paired_ptr<> _slot;
    std::string _connection_name;
};

template<typename... Args>
class signal
{
public:
    signal() = default;
    signal(const std::string &signal_name) : _signal_name{ signal_name } {}
    signal(signal &&other) = default;
    signal &operator=(signal &&other) = default;

    void emit(const Args &... args)
    {
        std::scoped_lock lock(_emit_lock);

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
        connection to_return(new_slot, _signal_name);

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

    void set_signal_name(const std::string &signal_name)
    {
        _signal_name = signal_name;
    }

    std::string_view get_signal_name() const
    {
        return _signal_name;
    }

private:
    std::string _signal_name;
    std::mutex _connect_lock, _emit_lock;
    std::vector<slot<Args...>> _slots, _pending_connections;
};
}
}
