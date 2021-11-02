#include <glad/glad.h>

#include "../toolbox/vector.hpp"
#include "light.hpp"

Light::Light()
{
    position.set(0, 0, 0);
    color.set(1, 1, 1);
    direction.set(0, -1, 0);
}
