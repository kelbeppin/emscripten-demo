
#include <stdexcept>
#include "shader.hpp"

//------------------------------------------------------------------------------

xyz::shader::shader(char const *source, GLenum type)
    : id_(0)
{
    id_ = glCreateShader(type);

    if (!id_) {
        throw std::runtime_error("glCreateShader() failed.");
    }

    glShaderSource(id_, 1, &source, nullptr);
    glCompileShader(id_);

    GLint status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &status);

    if (!status) {
        char buffer[256];
        glGetShaderInfoLog(id_, sizeof(buffer), nullptr, buffer);

        throw std::runtime_error(buffer);
    }
}

xyz::shader::shader(xyz::shader &&other) noexcept
    : id_(std::exchange(other.id_, 0))
{
}

xyz::shader::~shader() noexcept
{
    if (id_) {
        glDeleteShader(id_);
    }
}

xyz::shader &xyz::shader::operator=(xyz::shader &&other) noexcept
{
    if (this != &other) {
        id_ = std::exchange(other.id_, 0);
    }

    return *this;
}

GLuint xyz::shader::id() const
{
    return id_;
}

//------------------------------------------------------------------------------

xyz::vertex_shader::vertex_shader(char const *source)
    : shader(source, GL_VERTEX_SHADER)
{
}

//------------------------------------------------------------------------------

xyz::fragment_shader::fragment_shader(char const *source)
    : shader(source, GL_FRAGMENT_SHADER)
{
}

//------------------------------------------------------------------------------

xyz::program::program(xyz::vertex_shader const &vs,
    xyz::fragment_shader const &fs,
    std::vector<xyz::attrib_pair> const &attribs)
    : id_(0)
{
    id_ = glCreateProgram();

    if (!id_) {
        throw std::runtime_error("glCreateProgram() failed.");
    }

    for (auto it = attribs.begin(); it != attribs.end(); it++) {
        glBindAttribLocation(id_, it->index, it->name);
    }

    glAttachShader(id_, vs.id());
    glAttachShader(id_, fs.id());
    glLinkProgram(id_);

    GLint status;
    glGetProgramiv(id_, GL_LINK_STATUS, &status);

    if (!status) {
        char buffer[256];
        glGetProgramInfoLog(id_, sizeof(buffer), nullptr, buffer);

        throw std::runtime_error(buffer);
    }
}

xyz::program::program(xyz::program &&other) noexcept
    : id_(std::exchange(other.id_, 0))
{
}

xyz::program::~program() noexcept
{
    if (id_) {
        glDeleteProgram(id_);
    }
}

xyz::program &xyz::program::operator=(xyz::program &&other) noexcept
{
    if (this != &other) {
        id_ = std::exchange(other.id_, 0);
    }

    return *this;
}

GLuint xyz::program::id() const
{
    return id_;
}
