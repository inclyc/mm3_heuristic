add_requires("fmt", {system = true})

add_rules("mode.debug", "mode.release")
target("test_MST")
    set_kind("binary")
    add_files("MST.cpp")
    add_deps("Heuristic")
    add_packages("fmt")
