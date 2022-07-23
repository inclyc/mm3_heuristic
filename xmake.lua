includes("test")

add_rules("mode.debug", "mode.release")
target("dynamic_graph")
    set_kind("shared")
    add_includedirs("include")
    add_files("dynamic_graph/*.cpp")
    add_cxflags("-Weverything")