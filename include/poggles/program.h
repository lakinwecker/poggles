#pragma once

#include <array>
#include <filesystem>
#include <span>
#include <string>

#include "poggles/handle.h"
#include "poggles/poggles_export.hpp"
#include "poggles/shader.h"

namespace poggles {

class POGGLES_EXPORT shader_link_exception : public std::runtime_error {
  public:
    explicit shader_link_exception(char const *message)
        : std::runtime_error(message) {}

    explicit shader_link_exception(std::string const &message)
        : std::runtime_error(message) {}
};

class POGGLES_EXPORT program {
  public:
    program(
        std::vector<shader_desc> const &shader,
        // defines provided in the form "NAME [optional value]"
        std::vector<std::string> const &defines = {}
    );
    program(
        std::filesystem::path const &vertex_path,
        std::filesystem::path const &fragment_path,
        // defines provided in the form "NAME [optional value]"
        std::vector<std::string> const &defines = {}
    );

    // Public interface
    auto recompile() -> bool;
    auto use() const -> void;

    auto attach(shader_id id) -> void;

    explicit operator program_id() const { return static_cast<program_id>(m_program_handle); }

    auto set_bool(std::string const &name, bool value) const -> void;
    auto set_int(std::string const &name, int value) const -> void;
    auto set_texture_sampler(std::string const &name, GLint texture_num) const -> void;
    auto set_uint(std::string const &name, unsigned value) const -> void;
    auto set_float(std::string const &name, float value) const -> void;

    void set_vec2(std::string const &name, std::span<float const, 2> value) const;
    void set_vec3(std::string const &name, std::span<float const, 3> value) const;
    void set_vec4(std::string const &name, std::span<float const, 4> value) const;
    void set_mat4(std::string const &name, std::span<float const, 16> value) const;  // NOLINT

    auto set_double(std::string const &name, double value) const -> void;
    auto set_dvec3(std::string const &name, std::span<double const, 3> value) const -> void;
    auto set_dmat4(std::string const &name, std::span<double const, 16> value) const -> void;

    void set_uvec2(std::string const &name, std::span<const std::uint32_t, 2> value) const;
    void set_uvec3(std::string const &name, std::span<const std::uint32_t, 3> value) const;
    void set_uvec4(std::string const &name, std::span<const std::uint32_t, 4> value) const;

    template<size_t N>
    void set_float_array(std::string const &name, int count, std::array<float, N> array) const {
        glUniform1fv(
            glGetUniformLocation(static_cast<program_id>(m_program_handle), name.data()),
            count,
            array.data()
        );
    }

  private:
    program_handle m_program_handle;

    std::filesystem::path m_vertex_path;
    std::filesystem::path m_fragment_path;
};

auto checkLinkSuccess(program_id identifier) -> bool;

auto compileProgram(
    program_id program,
    std::vector<shader_desc> const &shaderFiles,
    // defines provided in the form "NAME [optional value]"
    std::vector<std::string> const &defines = {}
) -> bool;

}  // namespace poggles
