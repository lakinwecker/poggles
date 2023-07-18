#include <doctest/doctest.h>
#include <poggles/shader.h>

TEST_CASE("Shader define insertion") {
    std::string shaderSource = R"(
// This comment exists to pad the file

#version 410
layout(location = 0) in vec3 vertexPosition;

int main()
{
#ifdef NECESSARY_TO_COMPILE
    gl_Position = vec4(vertexPosition, 1);
}
)";

    std::string generatedSource = poggles::addDefinesToShaderSource(
        shaderSource, {"NECESSARY_TO_COMPILE", "JUST_FOR_FUN 1"}
    );
    std::string expectedSource = R"(
// This comment exists to pad the file

#version 410
#define NECESSARY_TO_COMPILE
#define JUST_FOR_FUN 1
layout(location = 0) in vec3 vertexPosition;

int main()
{
#ifdef NECESSARY_TO_COMPILE
    gl_Position = vec4(vertexPosition, 1);
}
)";

    CHECK(generatedSource == expectedSource);
}
