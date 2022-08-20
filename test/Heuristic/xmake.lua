add_requires("fmt", {system = true})

add_rules("mode.debug", "mode.release")
target("test_MST")
    set_kind("binary")
    add_files("MST.cpp")
    add_deps("Heuristic")
    add_packages("fmt")

add_rules("mode.debug", "mode.release")
target("test_BruteForce")
    set_kind("binary")
    add_files("BruteForce.cpp")
    add_deps("Heuristic")
    add_packages("fmt")



add_rules("mode.debug", "mode.release")
target("bench_random")
    set_kind("binary")
    add_files("BenchRandom.cpp")
    add_deps("Heuristic")
    add_packages("fmt")
