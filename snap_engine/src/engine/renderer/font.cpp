#include "font.h"
#include <engine/logger/asserts.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <GLEW/glew.h>


font fonts::load_font(const char * file_path, u8 font_size)
{
	font result;

	static FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		LOG_ERROR("failed to initialize free type library >> can't load file at path " + std::string(file_path));
		return result;
	}

	FT_Face face;
	if (FT_New_Face(ft, file_path, 0, &face))
	{
		LOG_ERROR("can't load font at " + std::string(file_path));
		return result;
	}

	FT_Set_Pixel_Sizes(face, 0, font_size);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (char c = ASCII_CHAR_START; c < ASCII_CHAR_END; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			LOG_ERROR("failed to load character Glyph metrics at ASCII code" + std::to_string(c));
			continue;
		}

		u32 texId = 0;

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Glyph character{
				   texId,
				   glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				   face->glyph->advance.x
		};

		result.characters.insert(std::pair<char, Glyph>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return result;
}