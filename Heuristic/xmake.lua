add_requires("fmt", "openmp", "libomp")
add_rules("mode.debug", "mode.release")
target("Heuristic")
    set_kind("shared")
    add_files("*.cpp")
    add_deps("Random")
    add_deps("DynamicGraph")
    add_packages("fmt", "openmp", "libomp")
