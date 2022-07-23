includes("test")
set_languages("cxx17")
add_cxflags("-Weverything")

add_rules("mode.debug", "mode.release")
target("DynamicGraph")
    set_kind("shared")
    add_includedirs("include")
    add_files("DynamicGraph/*.cpp")
    