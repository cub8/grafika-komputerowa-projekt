#pragma once

namespace WorldConstraints {
    // update in case of changing map
    const float ASPECT_RATIO = 1766.0f / 924.0f;
    const float SCALE = 20.0f;

    // Positive x as at the right, negative x is at the left
    const float LEFT = -SCALE * ASPECT_RATIO + 5.0f;
    const float RIGHT = SCALE * ASPECT_RATIO - 5.0f;

    // Positive z is at the bottom, negative z at the back
    const float TOP = -SCALE + 15.0f;
    const float BOTTOM = SCALE + 5.0f;
}
