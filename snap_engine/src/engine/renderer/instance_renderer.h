#pragma once
#include <engine/defines.h>
#include <glm/glm.hpp>
#include <engine/renderer/shader.h>
#include <vector>

#define MAX_INSTANCES 1000

class instance_renderer
{
public:
	static void instance_renderer_init();
	static void push_quad(const glm::vec2& position, const glm::vec2& scale, const color& color);
	static void render_quads(bool reset = false); // reset is assigned to be true if data is pushed every frame

private:
	static std::vector<glm::mat4> model_matrices;
	static std::vector<glm::vec4> colors;
	static std::shared_ptr<shader> m_shader_program;
	static u32 vao;
	static u32 vbo;
	static u32 ebo;
	static u32 color_vbo;
	static u32 matrices_buffer;
	static u32 quad_instances_count;
};