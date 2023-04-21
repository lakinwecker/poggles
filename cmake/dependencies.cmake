function (poggles_find_dependencies_thirdparty)
    find_package(glad REQUIRED)
    find_package(stb REQUIRED)
    find_package(opengl_system REQUIRED)
    find_package(doctest REQUIRED)
endfunction()

function (poggles_dependency_targets)
    set(POGGLES_DEPENDENCY_TARGETS 
        glad::glad
        stb::stb
        opengl::opengl
        doctest::doctest
        PARENT_SCOPE)
    # NOTE: remove duplicates so it can be called more than once.
    list(REMOVE_DUPLICATES POGGLES_DEPENDENCY_TARGETS)
endfunction()
