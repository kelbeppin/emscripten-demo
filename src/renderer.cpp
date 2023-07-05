
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/projection.hpp>
#include "renderer.hpp"

//------------------------------------------------------------------------------

namespace
{
    auto vertices = {
        -1.f, +1.f, -1.f, -1.f, +1.f, +1.f, +1.f, +1.f, +1.f,
        +1.f, +1.f, +1.f, +1.f, +1.f, -1.f, -1.f, +1.f, -1.f,

        +1.f, -1.f, +1.f, +1.f, +1.f, +1.f, -1.f, +1.f, +1.f,
        -1.f, +1.f, +1.f, -1.f, -1.f, +1.f, +1.f, -1.f, +1.f,

        -1.f, -1.f, +1.f, -1.f, -1.f, -1.f, +1.f, -1.f, -1.f,
        +1.f, -1.f, -1.f, +1.f, -1.f, +1.f, -1.f, -1.f, +1.f,

        -1.f, -1.f, -1.f, -1.f, +1.f, -1.f, +1.f, +1.f, -1.f,
        +1.f, +1.f, -1.f, +1.f, -1.f, -1.f, -1.f, -1.f, -1.f,

        +1.f, -1.f, -1.f, +1.f, +1.f, -1.f, +1.f, +1.f, +1.f,
        +1.f, +1.f, +1.f, +1.f, -1.f, +1.f, +1.f, -1.f, -1.f,

        -1.f, -1.f, +1.f, -1.f, +1.f, +1.f, -1.f, +1.f, -1.f,
        -1.f, +1.f, -1.f, -1.f, -1.f, -1.f, -1.f, -1.f, +1.f,
    };

    auto vertex_shader_source = R"(
        #version 100

        attribute vec3 aPosition;
        varying vec3 vPosition;
        uniform mat4 uProjection;
        uniform mat4 uView;
        uniform mat4 uModel;

        void main()
        {
            vPosition = aPosition;
            gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0);
        }
    )";

    auto fragment_shader_source = R"(
        #version 100

        precision mediump float;
        varying vec3 vPosition;

        void main()
        {
            vec3 color = (vPosition * 0.5) + vec3(0.5);
            gl_FragColor = vec4(color, 1.0);
        }
    )";

    template<typename T>
    T random(T min, T max)
    {
        T x = static_cast<T>(std::rand()) / static_cast<T>(RAND_MAX);
        return x * (max - min) + min;
    }
}

//------------------------------------------------------------------------------

xyz::renderer::renderer(xyz::bus const &bus)
    : bus_(bus)
    , program_(xyz::vertex_shader(vertex_shader_source),
        xyz::fragment_shader(fragment_shader_source),
        { { 0, "aPosition" } })
    , vbo_(0)
    , aspect_ratio_(1.f)
    , cubes_{}
{
    std::printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));
    std::printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    std::printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    std::printf("GL_EXTENSIONS: %s\n", glGetString(GL_EXTENSIONS));

    glClearColor(0.f, 0.f, 0.f, 1.f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glUseProgram(program_.id());

    update_projection();
    update_view();
    
    auto model = glm::identity<glm::mat4>();
    glUniformMatrix4fv(glGetUniformLocation(program_.id(), "uModel"),
        1, GL_FALSE, glm::value_ptr(model));
    
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.begin(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, reinterpret_cast<void *>(0));

    for (int i = 0; i < 1000; i++) {
        cube_info cube;

        cube.position = cube.initial_position = {
            random(-500.f, 500.f),
            random(-25.f, 25.f),
            random(-500.f, 500.f),
        };

        cube.rotation = cube.initial_rotation = {
            random(0.f, 360.f),
            random(0.f, 360.f),
            random(0.f, 360.f),
        };

        cube.bounce_factor = random(0.f, 1.f);

        cubes_.emplace_back(cube);
    }
}

xyz::renderer::~renderer() noexcept
{
    glDeleteBuffers(1, &vbo_);
}

void xyz::renderer::draw(float t)
{
    if (bus_.is_yaw_updated() || bus_.is_pitch_updated()) {
        update_view();
    }

    if (bus_.is_zoom_updated()) {
        update_projection();
    }

    if (bus_.is_window_size_updated()) {
        auto w = bus_.window_width();
        auto h = bus_.window_height();

        glViewport(0, 0, w, h);
        aspect_ratio_ = static_cast<float>(w) / static_cast<float>(h);
        update_projection();
    }

    auto model = glm::identity<glm::mat4>();
    glUniformMatrix4fv(glGetUniformLocation(program_.id(), "uModel"),
        1, GL_FALSE, glm::value_ptr(model));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 3));

    for (auto it = cubes_.begin(); it != cubes_.end(); it++) {
        it->position.y = it->initial_position.y + std::sin(t / (600.f + 1000.f * it->bounce_factor)) * 10.f;
        it->rotation.y = it->initial_rotation.y + t / 30.f;

        auto model = glm::identity<glm::mat4>();
        model = glm::translate(model, it->position);
        model = glm::rotate(model, glm::radians(it->rotation.x), glm::vec3(1.f, 0.f, 0.f));
        model = glm::rotate(model, glm::radians(it->rotation.y), glm::vec3(0.f, 1.f, 0.f));
        model = glm::rotate(model, glm::radians(it->rotation.z), glm::vec3(0.f, 0.f, 1.f));

        glUniformMatrix4fv(glGetUniformLocation(program_.id(), "uModel"),
            1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 3));
    }
}

void xyz::renderer::update_projection()
{
    auto projection = glm::perspective(glm::radians(bus_.zoom()), aspect_ratio_, 0.01f, 1000.f);

    glUniformMatrix4fv(glGetUniformLocation(program_.id(), "uProjection"),
        1, GL_FALSE, glm::value_ptr(projection));
}

void xyz::renderer::update_view()
{
    auto pos = glm::vec3(
        glm::cos(glm::radians(bus_.yaw())) * glm::cos(glm::radians(bus_.pitch())),
        glm::sin(glm::radians(bus_.pitch())),
        glm::sin(glm::radians(bus_.yaw())) * glm::cos(glm::radians(bus_.pitch()))
    ) * 10.f;

    auto target = glm::vec3(0.f, 0.f, 0.f);
    auto up = glm::vec3(0.f, 1.f, 0.f);

    auto view = glm::lookAt(pos, target, up);

    glUniformMatrix4fv(glGetUniformLocation(program_.id(), "uView"),
        1, GL_FALSE, glm::value_ptr(view));
}
