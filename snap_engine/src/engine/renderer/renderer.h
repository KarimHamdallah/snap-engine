#pragma once
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <engine/defines.h>
#include "shader.h"
#include <engine/physics/physics.h>
#include "font.h"

enum class pivot
{
	center = 0,
	bottom_left = 1,
	top_left = 2,
	bottom = 3,
	top = 4
};

struct render_object
{
	u32 vao;
	u32 vbo;
	u32 ebo;
};

class renderer
{
public:
	static bool renderer_init(void);
	static void renderer_begin(color clear_color = color{ 0.08f, 0.1f, 0.1f, 1.0f });
	static void renderer_end(SDL_Window* rendering_window);

	static void render_quad(glm::vec2 position, glm::vec2 scale, color color, pivot pivot_point = pivot::center);
	static void render_quad(glm::vec2 position, glm::vec2 scale, f32 rotation, color color);
	static void render_line(const glm::vec2& start, const glm::vec2& end, const color& color, f32 lineWidth = 1.0f);
	static void render_wideline(const glm::vec2& start, const glm::vec2& end, const color& color, f32 lineWidth = 0.5f);
	static void render_quadline(glm::vec2 position, glm::vec2 scale, color color, f32 lineWidth = 1.0f);
	static void render_aabb(const AABB& _aabb, color color, f32 lineWidth = 1.0f);

	static void render_smoothcircle(const glm::vec2& position, const glm::vec2& scale, const color& color = WHITE, f32 thickness = 1.0f, f32 fade = 0.005f);
	static void render_smoothcircle(const glm::vec2& position, f32 diameter, const color& color = WHITE, f32 thickness = 1.0f);

	static void render_polygonline(const std::vector<glm::vec2>& vertices, color color, f32 lineWidth = 1.0f);
	static void render_polygonline(glm::vec2* vertices, u32 vertices_count, color color, f32 lineWidth = 1.0f);

	static void render_text(font* font, const std::string& text, const glm::vec2& position, const glm::vec2& scale, const color& color);

	static void free_memory();
private:
	static std::shared_ptr<shader> m_default_shaderprogram;
	static std::shared_ptr<shader> m_smoothcircle_shaderprogram;
	static std::shared_ptr<shader> m_smoothline_shaderprogram;
	static std::shared_ptr<shader> m_text_shaderprogram;
	static render_object quad;
	static render_object quad_bottomleft;
	static render_object quad_topleft;
	static render_object quad_bottom;
	static render_object quad_top;
	static render_object line;
	static render_object smoothcircle;
	static render_object smoothline;
	static render_object textChar;
};

class rendererFactory
{
public:
	static render_object init_quad(pivot pivot_point = pivot::center);
	static render_object init_line();
	static render_object init_smoothcircle(); // return quad to draw circle inside it with shaders
	static render_object init_smoothline(); // return quad to draw line inside it with shaders
	static render_object init_textChar(); // return quad to draw charcater texture inside it with shaders
private:
};