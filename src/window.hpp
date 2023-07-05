
#pragma once

#include <SDL.h>
#include "bus.hpp"

namespace xyz
{
    class window final
    {
    public:
        window(xyz::bus &bus_);
        ~window() noexcept;

        bool process_events();
        void swap();

        xyz::bus const &bus() const;

    private:
        xyz::bus &bus_;
        SDL_Window *window_;
        SDL_GLContext gl_context_;
    };
}
