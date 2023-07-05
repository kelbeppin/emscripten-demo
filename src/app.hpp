
#include "renderer.hpp"
#include "window.hpp"

namespace xyz
{
    class app final
    {
    public:
        app();
        ~app() noexcept;

        void run();
        bool loop();

    private:
        xyz::bus bus_;
        xyz::window window_;
        xyz::renderer renderer_;
    };
}
