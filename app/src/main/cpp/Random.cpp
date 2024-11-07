#include "Random.h"

namespace Tools
{
    Random::Random()
            : mersenne_engine{rnd_device()} {
    }
}
