#pragma once
#include <engine/defines.h>

class math
{
	math() = delete;
	~math() = delete;

public:
	static f32 clampf(f32 value, f32 min, f32 max);
};