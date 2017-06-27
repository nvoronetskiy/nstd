solution "nstd_examples"
    configurations { "Debug", "Release" }
    userincludedirs { "../include" }
    flags { "StaticRuntime" }
    language "C++"

    project "asio_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        userincludedirs { "../include/external/asio/asio/include" }
        files { "asio_example.cpp" };
        links { iif(os.is("windows"), "ws2_32", ""), iif(os.is("windows"), "wsock32", "") }
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/asio_example/Debug"
            targetdir "bin/asio_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/asio_example/Release"
            targetdir "bin/asio_example/Release"

    project "base64_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "base64_example.cpp" };
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/base64_example/Debug"
            targetdir "bin/base64_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/base64_example/Release"
            targetdir "bin/base64_example/Release"

    project "date_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "date_example.cpp" };
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/date_example/Debug"
            targetdir "bin/date_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/date_example/Release"
            targetdir "bin/date_example/Release"

    project "expiry_cache_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "expiry_cache_example.cpp" };
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/expiry_cache_example/Debug"
            targetdir "bin/expiry_cache_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/expiry_cache_example/Release"
            targetdir "bin/expiry_cache_example/Release"

    project "live_property_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "live_property_example.cpp" };
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/live_property_example/Debug"
            targetdir "bin/live_property_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/live_property_example/Release"
            targetdir "bin/live_property_example/Release"

    project "relinx_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "relinx_example.cpp" };
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/relinx_example/Debug"
            targetdir "bin/relinx_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            linkoptions { "-s" }
            buildoptions { "-Wno-unused-variable", "-Wno-unused-but-set-variable" }
            flags { "OptimizeSpeed" }
            objdir "obj/relinx_example/Release"
            targetdir "bin/relinx_example/Release"

    project "relinx_generator_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        userincludedirs { "../include/external/asio/asio/include" }
        files { "relinx_generator_example.cpp" };
        links { iif(os.is("windows"), "ws2_32", ""), iif(os.is("windows"), "wsock32", "") }
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/relinx_generator_example/Debug"
            targetdir "bin/relinx_generator_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/relinx_generator_example/Release"
            targetdir "bin/relinx_generator_example/Release"

    project "sqlite_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall", "-Wno-unused-but-set-variable" }
        files { "sqlite_example.cpp", "../include/external/sqlite/sqlite3.c" };
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/sqlite_example/Debug"
            targetdir "bin/sqlite_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            linkoptions { "-s" }
            flags { "OptimizeSpeed" }
            objdir "obj/sqlite_example/Release"
            targetdir "bin/sqlite_example/Release"

    project "urdl_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        userincludedirs { "../include/external/asio/asio/include" }
        files { "urdl_example.cpp" };
        links { iif(os.is("windows"), "ws2_32", ""), iif(os.is("windows"), "wsock32", "") }
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/urdl_example/Debug"
            targetdir "bin/urdl_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            flags { "OptimizeSpeed" }
            linkoptions { "-s" }
            objdir "obj/urdl_example/Release"
            targetdir "bin/urdl_example/Release"

    project "uuid_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        userincludedirs { "../include/external/asio/asio/include" }
        files { "uuid_example.cpp" };
        links { iif(os.is("windows"), "ws2_32", ""), iif(os.is("windows"), "wsock32", "") }
        linkoptions { "-static" }
        configuration { "Debug" }
            flags { "Symbols" }
            objdir "obj/uuid_example/Debug"
            targetdir "bin/uuid_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            linkoptions { "-s" }
            flags { "OptimizeSpeed" }
            objdir "obj/uuid_example/Release"
            targetdir "bin/uuid_example/Release"
