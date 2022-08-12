#include "math.h"
#include <engine/logger/asserts.h>

f32 math::clampf(f32 value, f32 min, f32 max)
{
	LOG_ASSERT_MSG(min < max, "min value is larger than max value");

	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}