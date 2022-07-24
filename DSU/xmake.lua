add_rules("mode.debug", "mode.release")
target("DSU")
    set_kind("shared")
    add_files("*.cpp")