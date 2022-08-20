add_rules("mode.debug", "mode.release")
target("Random")
    set_kind("shared")
    add_files("*.cpp")
