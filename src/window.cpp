
#include <stdexcept>
#include "window.hpp"

#ifndef __EMSCRIPTEN__
#include <glad/glad.h>
#endif

//------------------------------------------------------------------------------

xyz::window::window(xyz::bus &bus)
    : bus_(bus)
    , window_(nullptr)
    , gl_context_(nullptr)
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("Failed to initialize SDL.");
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    window_ = SDL_CreateWindow(
        "emscripten-demo",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        512, 512,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window_) {
        throw std::runtime_error("Failed to create window.");
    }

    gl_context_ = SDL_GL_CreateContext(window_);

    if (!gl_context_) {
        throw std::runtime_error("Failed to create OpenGL context.");
    }

#ifndef __EMSCRIPTEN__
    if (!gladLoadGLES2Loader(SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to load OpenGL functions.");
    }
#endif
}

xyz::window::~window() noexcept
{
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyWindow(window_);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool xyz::window::process_events()
{
    SDL_Event event;

    bus_.reset();

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return false;
        case SDL_MOUSEMOTION:
            if (event.motion.state & SDL_BUTTON_LMASK) {
                bus_.add_yaw(static_cast<float>(event.motion.xrel));
                bus_.add_pitch(static_cast<float>(-event.motion.yrel));
            }
            break;
        case SDL_MOUSEWHEEL:
            bus_.add_zoom(static_cast<float>(-event.wheel.preciseY));
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                bus_.set_window_size(event.window.data1, event.window.data2);
            }
            break;
        }
    }

    return true;
}

void xyz::window::swap()
{
    SDL_GL_SwapWindow(window_);
}
