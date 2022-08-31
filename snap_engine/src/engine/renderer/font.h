#pragma once
#include <engine/defines.h>
#include <glm/glm.hpp>
#include <map>

#define ASCII_CHAR_START 32
#define ASCII_CHAR_END 127


// cahracter metrics
struct Glyph
{
	u32 texId;
	glm::ivec2 size;
	glm::ivec2 bearing;
	u32 advance;
};

class font
{
	font() = default;
	font(const char* file_path, u8 font_size);
	~font();
	std::map<char, Glyph> characters;
};