#pragma once
#include <engine/defines.h>
#include <glm/glm.hpp>

#define PI 3.14159265359
#define TO_RAD(degree) degree * 0.01745329251

class math
{
	math() = delete;
	~math() = delete;

public:
	static f32 clampf(f32 value, f32 min, f32 max);
	static f32 min(f32 a, f32 b);
	static f32 max(f32 a, f32 b);
	static f32 lengthSqrd(const glm::vec2& v);
	static f32 lerp(f32 a, f32 b, f32 t);
	static color lerp(const color& a, const color& b, f32 t);
	static glm::vec2 lerp(const glm::vec2& a, const glm::vec2& b, f32 t);
	static glm::vec2 transform_vector(const glm::vec2& vector, const glm::vec2& translation, const glm::vec2& scale, f32 rotation);
	// rotate vector around origin
	static glm::vec2 rotate_vector(const glm::vec2 & vector, f32 rotation);
	// rotate vector around certain point
	static glm::vec2 rotate_vector(const glm::vec2 & vector, const glm::vec2& point, f32 rotation);
	// get vector with length and in angle direction
	static glm::vec2 get_vector(f32 length, f32 angle);
};