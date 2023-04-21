function (poggles_declare_library BASE_DIR)
    add_library(
        poggles_poggles
        "${BASE_DIR}source/poggles/buffer.cpp"
        "${BASE_DIR}source/poggles/debug.cpp"
        "${BASE_DIR}source/poggles/program.cpp"
        "${BASE_DIR}source/poggles/shader.cpp"
        "${BASE_DIR}source/poggles/stb_image.cpp"
        "${BASE_DIR}source/poggles/vertex_array.cpp"
        "${BASE_DIR}source/poggles/texture.cpp"
        "${BASE_DIR}source/poggles/gl_function.cpp"
    )
    add_library(poggles::poggles ALIAS poggles_poggles)

    include(GenerateExportHeader)
    generate_export_header(
        poggles_poggles
        BASE_NAME poggles
        EXPORT_FILE_NAME export/poggles/poggles_export.hpp
        CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
    )

    if(NOT BUILD_SHARED_LIBS)
      target_compile_definitions(poggles_poggles PUBLIC POGGLES_STATIC_DEFINE)
    endif()

    set_target_properties(
        poggles_poggles PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
        EXPORT_NAME poggles
        OUTPUT_NAME poggles
    )

    target_include_directories(
        poggles_poggles ${warning_guard}
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
    )

    target_include_directories(
        poggles_poggles SYSTEM
        PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
    )

    target_compile_features(poggles_poggles PUBLIC cxx_std_20)

    target_link_libraries(
        poggles_poggles PRIVATE
        glad::glad
        stb::stb
        opengl::opengl
    )

endfunction()
