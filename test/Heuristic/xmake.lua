add_rules("mode.debug", "mode.release")
target("test_Heuristic")
    add_files("*.cpp")
    add_deps("Heuristic")