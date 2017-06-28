# nstd

This is a C++17 utility library. nstd stands for non-standard.
The library currently contains the following features:
   * signal_slot    - a signal/slot implementation that is thread safe and supports auto-disconnection from slots.
   * live_property - a templated wrapper class that emits signals when a value is changing/changed.
   * expiry_cache  - a container where a data can expire.
   * json - Niels Lohmann's json library [https://github.com/nlohmann/json]
   * asio - boost's networking library, but no dependancy on boost [http://think-async.com/]
   * urdl - the library to download web content (depends on asio) [https://github.com/chriskohlhoff/urdl]
   * date - the date and time manipulations library created by Howard Hinnant [https://github.com/HowardHinnant/date]
   * units - r-lyeh's the units calculations library [https://github.com/r-lyeh/units]
   * base64 - base64 encoding/decoding
   * uuid - unique id generator
   * relinx - LINQ-like data transformations
   * sqlite3/sqlite c++ wrapper - sql database support [https://github.com/aminroosta/sqlite_modern_cpp]
   * quantum random number provider (using QRNG internet service: http://qrng.anu.edu.au)
   * wire - r-lyeh's drop-in std::string replacement with extended functionality and safe C/C++ formatters [https://github.com/r-lyeh/wire]
   * giant - r-lyeh's tiny library to handle little/big endianness [https://github.com/r-lyeh/giant]
