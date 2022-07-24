add_rules("mode.debug", "mode.release")
target("DynamicGraph")
    set_kind("shared")
    add_files("*.cpp")