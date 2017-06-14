# nstd

This is a C++17 utility library. nstd stands for non-standard.
The library currently contains the following features:
   * signal_slot    - a signal/slot implementation that is thread safe and supports auto-disconnection from slots.
   * live_property - a templated wrapper class that emits signals when a value is changing/changed.
   * expiry_cache  - a container where a data can expire.
   * json - Niels Lohmann's json library
   * asio - boost's networking library, but no dependancy on boost
   * base64 - base64 encoding/decoding
   * uuid - unique id generator
   * relinx - LINQ-like data transformations
