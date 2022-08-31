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

struct font
{
	font() = default;
	~font() {}
	std::map<char, Glyph> characters;
};


class fonts
{
public:
	static font load_font(const char* file_path, u8 font_size);
};