includes("test")
set_languages("cxx20")
add_cxflags("-Weverything")
add_cxxflags("-Wno-c++98-compat")
add_cxxflags("-Wno-c++98-compat-pedantic")

add_rules("mode.debug", "mode.release")
target("DynamicGraph")
    set_kind("shared")
    add_includedirs("include")
    add_files("DynamicGraph/*.cpp")
    