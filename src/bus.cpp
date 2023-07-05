
#include "bus.hpp"

//------------------------------------------------------------------------------

xyz::bus::bus()
    : yaw_(45.f)
    , pitch_(30.f)
    , zoom_(45.f)
    , window_width_(0)
    , window_height_(0)
    , yaw_updated_(false)
    , pitch_updated_(false)
    , zoom_updated_(false)
    , window_size_updated_(false)
{
}

void xyz::bus::reset()
{
    yaw_updated_ = false;
    pitch_updated_ = false;
    zoom_updated_ = false;
    window_size_updated_ = false;
}

float xyz::bus::yaw() const
{
    return yaw_;
}

void xyz::bus::add_yaw(float delta)
{
    yaw_ += delta;
    yaw_updated_ = true;
}

float xyz::bus::pitch() const
{
    return pitch_;
}

void xyz::bus::add_pitch(float delta)
{
    pitch_ += delta;

    if (pitch_ < -89.f) {
        pitch_ = -89.f;
    } else if (pitch_ > 89.f) {
        pitch_ = 89.f;
    }

    pitch_updated_ = true;
}

float xyz::bus::zoom() const
{
    return zoom_;
}

void xyz::bus::add_zoom(float delta)
{
    zoom_ += delta;

    if (zoom_ < 5.f) {
        zoom_ = 5.f;
    } else if (zoom_ > 120.f) {
        zoom_ = 120.f;
    }

    zoom_updated_ = true;
}

int xyz::bus::window_width() const
{
    return window_width_;
}

int xyz::bus::window_height() const
{
    return window_height_;
}

void xyz::bus::set_window_size(int width, int height)
{
    window_width_ = width;
    window_height_ = height;
    window_size_updated_ = true;
}

bool xyz::bus::is_yaw_updated() const
{
    return yaw_updated_;
}

bool xyz::bus::is_pitch_updated() const
{
    return pitch_updated_;
}

bool xyz::bus::is_zoom_updated() const
{
    return zoom_updated_;
}

bool xyz::bus::is_window_size_updated() const
{
    return window_size_updated_;
}
