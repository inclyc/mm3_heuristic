includes("DSU")

add_rules("mode.debug", "mode.release")
target("test")
    set_kind("binary")
    add_includedirs("$(projectdir)/include")
    add_files("*.cpp")
    add_deps("DynamicGraph")