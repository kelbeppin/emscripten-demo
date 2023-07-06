
#pragma once

#include <vector>

#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#else
#include <GLES2/gl2.h>
#endif

namespace xyz
{
    class shader
    {
    public:
        shader(char const *source, GLenum type);
        shader(xyz::shader const &) = delete;
        shader(xyz::shader &&) noexcept;

        xyz::shader &operator=(xyz::shader const &) = delete;
        xyz::shader &operator=(xyz::shader &&) noexcept;

        virtual ~shader() noexcept;

        GLuint id() const;

    private:
        GLuint id_;
    };

    class vertex_shader : public shader
    {
    public:
        vertex_shader(char const *source);
    };

    class fragment_shader : public shader
    {
    public:
        fragment_shader(char const *source);
    };

    struct attrib_pair
    {
        unsigned int index;
        char const *name;
    };

    class program final
    {
    public:
        program(
            xyz::vertex_shader const &vs,
            xyz::fragment_shader const &fs,
            std::vector<xyz::attrib_pair> const &attribs = {}
        );
        program(xyz::program const &) = delete;
        program(xyz::program &&) noexcept;

        xyz::program &operator=(xyz::program const &) = delete;
        xyz::program &operator=(xyz::program &&) noexcept;

        ~program() noexcept;

        GLuint id() const;

    private:
        GLuint id_;
    };
}
