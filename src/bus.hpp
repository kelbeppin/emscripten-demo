
#pragma once

namespace xyz
{
    class bus final
    {
    public:
        bus();

        void reset();

        float yaw() const;
        void add_yaw(float delta);

        float pitch() const;
        void add_pitch(float delta);

        float zoom() const;
        void add_zoom(float delta);

        int window_width() const;
        int window_height() const;
        void set_window_size(int width, int height);

        bool is_yaw_updated() const;
        bool is_pitch_updated() const;
        bool is_zoom_updated() const;
        bool is_window_size_updated() const;

    private:
        float yaw_;
        float pitch_;
        float zoom_;
        int window_width_;
        int window_height_;

        bool yaw_updated_;
        bool pitch_updated_;
        bool zoom_updated_;
        bool window_size_updated_;
    };
}
