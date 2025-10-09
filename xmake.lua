add_requires("glfw")
add_requires("glad")
add_requires("glm")

add_rules("mode.debug", "mode.release")

set_symbols("debug", "edit")
set_rundir(".")

if is_plat("windows") then
    set_runtimes(is_mode("debug") and "MDd" or "MD")
    add_cxflags("cl::/diagnostics:caret")
end

target("M45x3D")
    set_kind("binary")
    set_languages("c++17")
    add_headerfiles("src/**.h")
    add_files("src/**.cpp")
    add_packages("glfw")
    add_packages("glad")
    add_packages("glm")