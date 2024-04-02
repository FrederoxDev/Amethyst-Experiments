#include "dllmain.h"
#include <chrono>
#include <cmath>
#include <math.h>
#include <vector>

AmethystContext* amethyst;

ModFunction void Initialize(AmethystContext* _amethyst)
{
    amethyst = _amethyst;
}