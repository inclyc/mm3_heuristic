includes("test")
includes("DynamicGraph")
includes("DSU")
includes("Heuristic")
includes("Random")
add_includedirs("$(projectdir)/include")
set_languages("cxx20")
add_cxflags("-Wall")
add_cxflags("-pedantic")
add_cxflags("-Wextra")
add_cxxflags("-Wno-c++98-compat")
add_cxxflags("-Wno-c++98-compat-pedantic")

