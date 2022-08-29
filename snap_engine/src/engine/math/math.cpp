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

f32 math::min(f32 a, f32 b)
{
	if (a < b)
		return a;
	return b;
}

f32 math::max(f32 a, f32 b)
{
	if (a > b)
		return a;
	return b;
}

f32 math::lengthSqrd(const glm::vec2 & v)
{
	return v.x * v.x + v.y * v.y;
}

f32 math::lerp(f32 a, f32 b, f32 t)
{
	return a + (b - a) * t;
}

color math::lerp(const color& a, const color& b, f32 t)
{
	f32 red = lerp(a.r, b.r, t);
	f32 green = lerp(a.g, b.g, t);
	f32 blue = lerp(a.b, b.b, t);
	f32 alpha = lerp(a.a, b.a, t);
	
	return color{ red,green,blue,alpha };
}

glm::vec2 math::lerp(const glm::vec2 & a, const glm::vec2 & b, f32 t)
{
	f32 x = lerp(a.x, b.x, t);
	f32 y = lerp(a.y, b.y, t);
	return glm::vec2(x, y);
}

glm::vec2 math::transform_vector(const glm::vec2 & vector, const glm::vec2 & translation, const glm::vec2 & scale, f32 rotation)
{
	glm::vec2 result = vector;

	rotation = TO_RAD(rotation);
	// SRT

	// scale
	result.x *= scale.x;
	result.y *= scale.y;
	// roatate
	// scaled vector
	f32 scaled_x = result.x;
	f32 scaled_y = result.y;
	result.x = cosf(rotation) * scaled_x - sinf(rotation) * scaled_y;
	result.y = sinf(rotation) * scaled_x + cosf(rotation) * scaled_y;
	// translate
	result.x += translation.x;
	result.y += translation.y;

	return result;
}

glm::vec2 math::rotate_vector(const glm::vec2 & vector, f32 rotation)
{
	glm::vec2 result = vector;

	rotation = TO_RAD(rotation);
	// rotate
	f32 scaled_x = result.x;
	f32 scaled_y = result.y;
	result.x = cosf(rotation) * scaled_x - sinf(rotation) * scaled_y;
	result.y = sinf(rotation) * scaled_x + cosf(rotation) * scaled_y;

	return result;
}

glm::vec2 math::rotate_vector(const glm::vec2 & vector, const glm::vec2 & point, f32 rotation)
{
	f32 angle = TO_RAD(rotation);
	f32 x = vector.x;
	f32 y = vector.y;
	f32 dx = point.x;
	f32 dy = point.y;

	f32 x_rotated = ((x - dx) * cos(angle)) - ((y - dy) * sin(angle)) + dx;
	f32 y_rotated = ((x - dx) * sin(angle)) + ((y - dy) * cos(angle)) + dy;
	
	return glm::vec2(x_rotated, y_rotated);
}

glm::vec2 math::get_vector(f32 length, f32 angle)
{
	angle = TO_RAD(angle);
	f32 x = length * sin(angle);
	f32 y = length * cos(angle);
	return glm::vec2(x, y);
}