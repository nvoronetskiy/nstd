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

#include <string>
#include <vector>

namespace nstd
{
namespace base64
{

inline constexpr uint8_t basis_64[] { "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/" };

template<typename Container>
std::string base64_encode(const Container &data)
{
    auto bytes_to_encode { reinterpret_cast<const uint8_t*>(std::data(data)) };
    auto len { std::size(data) };
    std::vector<uint8_t> container((len + 2) / 3 * 4, 0);
    auto result { std::data(container) };
    uint64_t idx { 0 }, i { 0 };

	for (; i < len - 2; i += 3)
    {
		result[idx++] = basis_64[ (bytes_to_encode[i] >> 2) & 0x3F];
		result[idx++] = basis_64[((bytes_to_encode[i] & 0x3) << 4) | (int(bytes_to_encode[i + 1] & 0xF0) >> 4)];
		result[idx++] = basis_64[((bytes_to_encode[i + 1] & 0xF) << 2) | (int(bytes_to_encode[i + 2] & 0xC0) >> 6)];
		result[idx++] = basis_64[  bytes_to_encode[i + 2] & 0x3F];
	}

	if (i < len)
    {
		result[idx++] = basis_64[(bytes_to_encode[i] >> 2) & 0x3F];

		if (i == (len - 1))
        {
			result[idx++] = basis_64[((bytes_to_encode[i] & 0x3) << 4)];
			result[idx++] = '=';
		}
		else
        {
			result[idx++] = basis_64[((bytes_to_encode[i] & 0x3) << 4) | ((int)(bytes_to_encode[i + 1] & 0xF0) >> 4)];
			result[idx++] = basis_64[((bytes_to_encode[i + 1] & 0xF) << 2)];
		}

		result[idx++] = '=';
	}

    return std::string(result, result + idx);
}

template<typename RetType = std::vector<uint8_t>>
RetType base64_decode(const std::string &encoded_string)
{
    static constexpr const uint8_t base64_dtable[256]
    {
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 62  , 0x80, 62  , 0x80, 63  ,
        52  , 53  , 54  , 55  , 56  , 57  , 58  , 59  , 60  , 61  , 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0   , 1   , 2   , 3   , 4   , 5   , 6   , 7   , 8   , 9   , 10  , 11  , 12  , 13  , 14  ,
        15  , 16  , 17  , 18  , 19  , 20  , 21  , 22  , 23  , 24  , 25  , 0x80, 0x80, 0x80, 0x80, 63  ,
        0x80, 26  , 27  , 28  , 29  , 30  , 31  , 32  , 33  , 34  , 35  , 36  , 37  , 38  , 39  , 40  ,
        41  , 42  , 43  , 44  , 45  , 46  , 47  , 48  , 49  , 50  , 51  , 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
    };
    const uint8_t* p { reinterpret_cast<const uint8_t*>(std::data(encoded_string)) };
    auto len { std::size(encoded_string) };
	uint8_t pad = len > 0 && (len % 4 || p[len - 1] == '=');
	const size_t L = ((len + 3) / 4 - pad) * 4;
	RetType container(3 * ((len + 3) / 4), typename RetType::value_type());
	int j = 0;

	for (size_t i = 0; i < L; i += 4)
	{
		int n = base64_dtable[p[i]] << 18 | base64_dtable[p[i + 1]] << 12 | base64_dtable[p[i + 2]] << 6 | base64_dtable[p[i + 3]];
		container[j++] = n >> 16;
		container[j++] = n >> 8 & 0xFF;
		container[j++] = n & 0xFF;
	}
	if (pad)
	{
		int n = base64_dtable[p[L]] << 18 | base64_dtable[p[L + 1]] << 12;
		container[j++] = n >> 16;

		if (len > L + 2 && p[L + 2] != '=')
		{
			n |= base64_dtable[p[L + 2]] << 6;
			container[j++] = n >> 8 & 0xFF;
		}
	}

	container.resize(j);

	return container;
}

}
}
