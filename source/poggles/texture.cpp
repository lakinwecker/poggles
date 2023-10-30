#include "poggles/texture.h"

#include "stb_image.h"

#include <stdexcept>

// https://www.cppstories.com/2018/09/visit-variants/
template<class... Ts>
struct overload : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overload(Ts...) -> overload<Ts...>;

auto poggles::uploadFromFile(
    poggles::texture const &tex,
    GLenum target,
    std::filesystem::path const &filename,
    GLint level
) -> bool {
    int width     = 0;
    int height    = 0;
    int channels  = 0;
    uint8_t *data = stbi_load(filename.string().c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
        return false;
    }

    // Call ourselves, but with the relevant data extracted from the
    // file
    bool status = uploadFromData(tex, target, data, width, height, channels, level);
    stbi_image_free(data);
    return status;
}

auto poggles::uploadFromData(
    poggles::texture const &tex,
    GLenum target,
    std::variant<float *, uint8_t *> data,
    int width,
    int height,
    int channels,
    GLint level
) -> bool {
    glTexParameteri(tex.target(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(tex.target(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(tex.target(), GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex.target(), GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum format = GL_RGB;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 2) {
        format = GL_RG;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    }

    std::visit(
        overload{
            [&](float *pixelData) {
                glTexImage2D(
                    target,
                    level,
                    static_cast<GLint>(format),
                    width,
                    height,
                    0,
                    format,
                    GL_FLOAT,
                    pixelData
                );
            },
            [&](uint8_t *pixelData) {
                glTexImage2D(
                    target,
                    level,
                    static_cast<GLint>(format),
                    width,
                    height,
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    pixelData
                );
            }
        },
        data
    );
    return true;  // TODO: Consider how to handle errors better in poggles - this
                  // should be false on a glError
}

poggles::texture::texture(GLenum target)
    : m_target(target) {
    bind();
}

void poggles::texture::bind(GLenum texture_num) const {
    activate(texture_num);
    glBindTexture(m_target, static_cast<GLuint>(m_texture_handle.value()));
}

poggles::texture::operator GLuint() const { return static_cast<GLuint>(m_texture_handle.value()); }
