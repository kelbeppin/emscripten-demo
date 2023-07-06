
#include <memory>
#include <stdexcept>
#include "app.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

//------------------------------------------------------------------------------

xyz::app::app()
    : bus_()
    , window_(bus_)
    , renderer_(bus_)
{
}

xyz::app::~app() noexcept
{
}

void xyz::app::run()
{
#ifndef __EMSCRIPTEN__
    while (true) {
        if (!loop()) {
            break;
        }
    }
#else
    emscripten_set_main_loop_arg([](void *arg) {
        if (!reinterpret_cast<xyz::app *>(arg)->loop()) {
            emscripten_cancel_main_loop();
        }
    }, this, 0, true);
#endif
}

bool xyz::app::loop()
{
    if (!window_.process_events()) {
        return false;
    }

    renderer_.draw(static_cast<float>(SDL_GetTicks()));
    window_.swap();

    return true;
}

//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    try {
        auto app = std::make_unique<xyz::app>();
        app->run();
    } catch (std::exception const &e) {
        std::fprintf(stderr, "Exception: %s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
