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
        configuration { "Debug" }
            objdir "obj/asio_example/Debug"
            targetdir "bin/asio_example/Debug"

        configuration { "Release" }
            objdir "obj/asio_example/Release"
            targetdir "bin/asio_example/Release"

        configuration "windows"
            links { "ws2_32", "wsock32" }

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

    project "giant_example"
        files { "giant_example.cpp" };
        buildoptions { "-Wno-unused-variable" }
        configuration { "Debug" }
            objdir "obj/giant_example/Debug"
            targetdir "bin/giant_example/Debug"

        configuration { "Release" }
            objdir "obj/giant_example/Release"
            targetdir "bin/giant_example/Release"

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
        configuration { "Debug" }
            objdir "obj/relinx_generator_example/Debug"
            targetdir "bin/relinx_generator_example/Debug"

        configuration { "Release" }
            objdir "obj/relinx_generator_example/Release"
            targetdir "bin/relinx_generator_example/Release"

        configuration "windows"
            links { "ws2_32", "wsock32" }


    project "sqlite_example"
        files { "sqlite_example.cpp", "../include/external/sqlite/sqlite3.c" };
        buildoptions { "-Wno-unused-but-set-variable" }
        configuration { "Debug" }
            objdir "obj/sqlite_example/Debug"
            targetdir "bin/sqlite_example/Debug"

        configuration { "Release" }
            objdir "obj/sqlite_example/Release"
            targetdir "bin/sqlite_example/Release"

    project "units_example"
        files { "units_example.cpp" };
        configuration { "Debug" }
            objdir "obj/units_example/Debug"
            targetdir "bin/units_example/Debug"

        configuration { "Release" }
            objdir "obj/units_example/Release"
            targetdir "bin/units_example/Release"

    project "urdl_example"
        files { "urdl_example.cpp" };
        userincludedirs { "../include/external/asio/asio/include" }
        configuration { "Debug" }
            objdir "obj/urdl_example/Debug"
            targetdir "bin/urdl_example/Debug"

        configuration { "Release" }
            objdir "obj/urdl_example/Release"
            targetdir "bin/urdl_example/Release"

        configuration "windows"
            links { "ws2_32", "wsock32" }

    project "uuid_example"
        files { "uuid_example.cpp" };
        userincludedirs { "../include/external/asio/asio/include" }
        configuration { "Debug" }
            objdir "obj/uuid_example/Debug"
            targetdir "bin/uuid_example/Debug"

        configuration { "Release" }
            objdir "obj/uuid_example/Release"
            targetdir "bin/uuid_example/Release"

        configuration "windows"
            links { "ws2_32", "wsock32" }

    project "wire_example"
        files { "wire_example.cpp" };
        buildoptions { "-Wno-unused-variable" }
        configuration { "Debug" }
            objdir "obj/wire_example/Debug"
            targetdir "bin/wire_example/Debug"

        configuration { "Release" }
            objdir "obj/wire_example/Release"
            targetdir "bin/wire_example/Release"

