#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <engine/defines.h>
#include "shader.h"
#include <engine/physics/physics.h>

struct render_object
{
	u32 vao;
	u32 vbo;
	u32 ebo;
};

struct texture
{
	u32 id;
	u32 width;
	u32 height;
};

class renderer
{
public:
	static bool renderer_init(void);
	static void renderer_begin(color clear_color = color{ 0.08f, 0.1f, 0.1f, 1.0f });
	static void renderer_end(SDL_Window* rendering_window);

	static void render_quad(glm::vec2 position, glm::vec2 scale, color color);
	static void render_line(glm::vec2 start, glm::vec2 end, color color, f32 lineWidth = 1.0f);
	static void render_quadline(glm::vec2 position, glm::vec2 scale, color color, f32 lineWidth = 1.0f);
	static void render_aabb(const AABB& _aabb, color color, f32 lineWidth = 1.0f);

	static texture creat_texture(const char* file_path, bool flip = true);
	static void render_sprite(texture *tex, const glm::vec2& position, const glm::vec2& scale, const color& color = WHITE);
private:
	static std::shared_ptr<shader> m_default_shaderprogram;
	static std::shared_ptr<shader> m_sprite_shaderprogram;
	static render_object quad;
	static render_object line;
};

class rendererFactory
{
public:
	static render_object init_quad();
	static render_object init_line();
private:
};