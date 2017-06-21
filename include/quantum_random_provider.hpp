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

#include <algorithm>
#include <iterator>
#include <mutex>
#include "urdl.hpp"
#include "json.hpp"

namespace nstd
{

template<typename T = uint64_t>
class quantum_random_provider
{
public:
    quantum_random_provider()
    {
        std::scoped_lock lock{ _mutex };

        if (std::size(_quantum_data) < _cache_size)
        {
            _quantum_data.resize(_cache_size);
            preload_quantum_data();
        }
    }

    auto operator ()() -> decltype(auto)
    {
        std::scoped_lock lock{ _mutex };

        auto const data_begin { std::data(_quantum_data) };

        if (data_begin == nullptr) _quantum_data.resize(_cache_size);

        if (_current_ptr - data_begin + sizeof(T) >= 1024)
        {
            preload_quantum_data();
        }

        T result { *reinterpret_cast<T*>(_current_ptr) };

        _current_ptr += sizeof(T);

        return result;
    }

private:
    inline static constexpr const std::size_t _cache_size { 1024 };
    inline static std::vector<uint8_t> _quantum_data;
    inline static uint8_t *_current_ptr = nullptr;
    inline static std::mutex _mutex;
    inline static constexpr const char *_url { "http://qrng.anu.edu.au/API/jsonI.php?type=uint8&length=" };

    void preload_quantum_data()
    {
        auto const data_begin { std::data(_quantum_data) };
        auto const length_to_download { _current_ptr == nullptr ? _cache_size : _current_ptr - data_begin };
        auto const length_to_move { _cache_size - length_to_download };
        auto url { std::string(_url) + std::to_string(length_to_download) };
        auto json_str { nstd::download_url(url).second };
        auto json { nstd::json::json::parse(json_str) };
        auto &json_random_data { json[0]["data"] };

        std::move(_current_ptr, _current_ptr + length_to_move, data_begin);

        _current_ptr = data_begin;
        _current_ptr += length_to_move;

        std::transform(std::begin(json_random_data), std::end(json_random_data), _current_ptr, [](auto &&v) { return static_cast<uint8_t>(v); });
    }
};

}
