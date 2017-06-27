solution "nstd_examples"
    configurations { "Debug", "Release" }
    userincludedirs { "../include" }
    flags { "StaticRuntime" }
    buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
    linkoptions { "-static" }
    kind "ConsoleApp"
    language "C++"

    configuration { "Debug" }
        flags { "Symbols" }

    configuration { "Release" }
        defines { "NDEBUG" }
        flags { "OptimizeSpeed" }

    project "asio_example"
        files { "asio_example.cpp" };
        userincludedirs { "../include/external/asio/asio/include" }
        links { iif(os.is("windows"), { "ws2_32", "wsock32" }, {}) }
        configuration { "Debug" }
            objdir "obj/asio_example/Debug"
            targetdir "bin/asio_example/Debug"

        configuration { "Release" }
            objdir "obj/asio_example/Release"
            targetdir "bin/asio_example/Release"

    project "base64_example"
        files { "base64_example.cpp" };
        configuration { "Debug" }
            objdir "obj/base64_example/Debug"
            targetdir "bin/base64_example/Debug"

        configuration { "Release" }
            objdir "obj/base64_example/Release"
            targetdir "bin/base64_example/Release"

    project "date_example"
        files { "date_example.cpp" };
        configuration { "Debug" }
            objdir "obj/date_example/Debug"
            targetdir "bin/date_example/Debug"

        configuration { "Release" }
            objdir "obj/date_example/Release"
            targetdir "bin/date_example/Release"

    project "expiry_cache_example"
        files { "expiry_cache_example.cpp" };
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/expiry_cache_example/Debug"
            targetdir "bin/expiry_cache_example/Debug"

        configuration { "Release" }
            objdir "obj/expiry_cache_example/Release"
            targetdir "bin/expiry_cache_example/Release"

    project "live_property_example"
        files { "live_property_example.cpp" };
        configuration { "Debug" }
            objdir "obj/live_property_example/Debug"
            targetdir "bin/live_property_example/Debug"

        configuration { "Release" }
            objdir "obj/live_property_example/Release"
            targetdir "bin/live_property_example/Release"

    project "relinx_example"
        files { "relinx_example.cpp" };
        configuration { "Debug" }
            objdir "obj/relinx_example/Debug"
            targetdir "bin/relinx_example/Debug"

        configuration { "Release" }
            buildoptions { "-Wno-unused-variable", "-Wno-unused-but-set-variable" }
            objdir "obj/relinx_example/Release"
            targetdir "bin/relinx_example/Release"

    project "relinx_generator_example"
        files { "relinx_generator_example.cpp" };
        userincludedirs { "../include/external/asio/asio/include" }
        links { iif(os.is("windows"), { "ws2_32", "wsock32" }, {}) }
        configuration { "Debug" }
            objdir "obj/relinx_generator_example/Debug"
            targetdir "bin/relinx_generator_example/Debug"

        configuration { "Release" }
            objdir "obj/relinx_generator_example/Release"
            targetdir "bin/relinx_generator_example/Release"

    project "sqlite_example"
        files { "sqlite_example.cpp", "../include/external/sqlite/sqlite3.c" };
        buildoptions { "-Wno-unused-but-set-variable" }
        configuration { "Debug" }
            objdir "obj/sqlite_example/Debug"
            targetdir "bin/sqlite_example/Debug"

        configuration { "Release" }
            objdir "obj/sqlite_example/Release"
            targetdir "bin/sqlite_example/Release"

    project "urdl_example"
        files { "urdl_example.cpp" };
        userincludedirs { "../include/external/asio/asio/include" }
        links { iif(os.is("windows"), { "ws2_32", "wsock32" }, {}) }
        configuration { "Debug" }
            objdir "obj/urdl_example/Debug"
            targetdir "bin/urdl_example/Debug"

        configuration { "Release" }
            objdir "obj/urdl_example/Release"
            targetdir "bin/urdl_example/Release"

    project "uuid_example"
        files { "uuid_example.cpp" };
        userincludedirs { "../include/external/asio/asio/include" }
        links { iif(os.is("windows"), { "ws2_32", "wsock32" }, {}) }
        configuration { "Debug" }
            objdir "obj/uuid_example/Debug"
            targetdir "bin/uuid_example/Debug"

        configuration { "Release" }
            objdir "obj/uuid_example/Release"
            targetdir "bin/uuid_example/Release"
