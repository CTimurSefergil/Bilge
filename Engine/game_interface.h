#pragma once

#include "defines.h"

// Game interface function pointers
struct GameInterface
{
    b8 (*initialize)();
    b8 (*update)(f64 delta_time);
    void (*shutdown)();
    void (*render)();
};