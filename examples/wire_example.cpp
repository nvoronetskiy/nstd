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

#include <iostream>
#include "wire.hpp"

int main()
{
    using namespace nstd::wire;

    /* many constructors */
    {
        string helloworld( "hello world" );                   // -> "hello world"
        string h( 'h' );                                      // -> "h"
        string hhh2( string( 3, 'h' ) << "abc" );       // -> "hhhabc"
        string hhh = string( 'h', 3 ) << std::endl;     // -> "hhh\n"
        string minusone = -1;                                 // -> "-1"
        string zero = 0;                                      // -> "0"
        string boolean = false;                               // -> "false"
        string real = 3.1415926535897932384626433832795L;     // ~-> "3.14159"
    }

    /* safe c++ constructors */
    {
        string arg1( "hello \1", "world" );
        // -> "hello world"
        std::cout << arg1 << std::endl;

        string arg2( "hello \2 \1", "world", true );
        // -> "hello true world"
        std::cout << arg2 << std::endl;

        string arg3( "hello \1\2\3", '{', "world", (unsigned char)('}') );
        // -> "hello {world}"
        std::cout << arg3 << std::endl;

        string arg4( "hello \1 \2 \3 \4 \5 \6 \7", "world", 3.14159f, 3.14159L, false, '\x1', arg3, 0 );
        // -> "hello world 3.14159 3.14159 false \x1 hello {world} 0"
        std::cout << arg4 << std::endl;
    }

    /* chaining */
    {
        string chain;
        chain << "hello world: " << 3 << 'a' << -1 << std::endl;    // -> "hello world: 3a-1\n"
    }

    /* implicit type conversion */
    {
        std::string boolean = string(false);          // -> "false"
        bool f = string("false");                     // -> false
        int i = string("123");                        // -> 123
        int j = string(123);                          // -> 123
    }

    /* explicit type conversion */
    {
        bool t = string(100).as<bool>();              // -> true
        int k = string(-456.123).as<int>();           // -> -456
    }

    /* extended methods */
    {
        string hi("Hi!");
        hi.at( 0);         // -> 'H'
        hi.at( 1);         // -> 'i'
        hi.at( 2);         // -> '!'
        hi.at( 3);         // -> 'H'
        hi.at( 4);         // -> 'i'
        hi.at( 5);         // -> '!'
        // [...]
        hi.at(-1);         // -> '!'
        hi.at(-2);         // -> 'i'
        hi.at(-3);         // -> 'H'
        hi.at(-4);         // -> '!'
        hi.at(-5);         // -> 'i'
        hi.at(-6);         // -> 'H'
        // [...]
        hi.at(5) = '?';    // hi = 'Hi?'
        hi.push_back(404); // hi = "Hi?404"
        hi.push_back('!'); // hi = "Hi?404!"
        hi.push_back(hi);  // hi = "Hi?404!Hi?404!"
    }

    /* quick introspection echo macro */
    {
        int health = 100;
        float money = 123.25;
        const char *hello = "world!";
        std::string echo = $wire("\1 = \2\n", health,money,hello); // -> "health=100,money=123.25,hello=world!,"

        std::cout << std::endl << echo << std::endl;
    }

    return 0;
}
