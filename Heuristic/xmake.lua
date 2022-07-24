add_rules("mode.debug", "mode.release")
target("Heuristic")
    set_kind("shared")
    add_files("*.cpp")