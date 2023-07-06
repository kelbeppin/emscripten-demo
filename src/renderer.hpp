
#pragma once

#include <glm/glm.hpp>
#include "bus.hpp"
#include "shader.hpp"

namespace xyz
{
    struct cube_info
    {
        glm::vec3 initial_position;
        glm::vec3 initial_rotation;
        glm::vec3 position;
        glm::vec3 rotation;
        float bounce_factor;
    };

    class renderer final
    {
    public:
        renderer(xyz::bus const &bus);
        ~renderer() noexcept;

        void draw(float t);

    private:
        void update_projection();
        void update_view();

        xyz::bus const &bus_;
        xyz::program program_;
        GLuint vbo_;
        float aspect_ratio_;
        std::vector<xyz::cube_info> cubes_;
    };
}
