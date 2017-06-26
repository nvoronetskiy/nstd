solution "nstd_examples"
    configurations { "Debug", "Release" }
    userincludedirs { "../include" }
    flags { "StaticRuntime" }
    language "C++"

    project "relinx_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "relinx_example.cpp" };
        configuration { "Debug" }
            flags { "Symbols" }
            flags { "OptimizeSpeed" }
            objdir "obj/relinx_example/Debug"
            targetdir "bin/relinx_example/Debug"

        configuration { "Release" }
            defines { "NDEBUG" }
            linkoptions { "-s" }
            objdir "obj/relinx_example/Release"
            targetdir "bin/relinx_example/Release"

    project "live_property_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        files { "live_property_example.cpp" };
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

    project "asio_example"
        kind "ConsoleApp"
        buildoptions { "-std=c++1z", "-fexceptions", "-Wall" }
        userincludedirs { "../include/external/asio/asio/include" }
        files { "asio_example.cpp" };
        links { iif(os.is("windows"), "ws2_32", ""), iif(os.is("windows"), "wsock32", "") }
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

