#include "instance_renderer.h"
#include <GLEW/glew.h>
#include <engine/display/window.h>
#include <engine/logger/asserts.h>

std::vector<glm::mat4> instance_renderer::model_matrices;
std::vector<glm::vec4> instance_renderer::colors;
std::shared_ptr<shader> instance_renderer::m_shader_program;
u32 instance_renderer::vao;
u32 instance_renderer::vbo;
u32 instance_renderer::ebo;
u32 instance_renderer::color_vbo;
u32 instance_renderer::matrices_buffer;
u32 instance_renderer::quad_instances_count;

void instance_renderer::instance_renderer_init()
{
	m_shader_program = std::make_shared<shader>("assets/shaders/instance_vertex.glsl", "assets/shaders/instance_fragment.glsl");

	vao = 0;
	vbo = 0;
	ebo = 0;
	color_vbo = 0;
	matrices_buffer = 0;
	quad_instances_count = 0;

	f32 vertices[]
	{  // x      y     z     u    v
		 0.5f,  0.5f, 0.0f, 0.0f,0.0f,  // top right
		 0.5f, -0.5f, 0.0f, 0.0f,1.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 1.0f,1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f,0.0f  // top left 
	};

	u32 indices[]
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texcoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);







	// buffer object holding colors
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);

	// set color attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);







	// buffer object holding matrices data
	glGenBuffers(1, &matrices_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, matrices_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCES * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindVertexArray(vao);

	// vertex attributes
	std::size_t vec4Size = sizeof(glm::vec4);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));

	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void instance_renderer::push_quad(const glm::vec2 & position, const glm::vec2 & scale, const color & color)
{
	if (quad_instances_count > MAX_INSTANCES)
	{
		LOG_ERROR("can't push quad any more .... max instances count is {}", MAX_INSTANCES);
		return;
	}
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	model_matrices.push_back(model);

	colors.push_back(glm::vec4((f32)color.r, (f32)color.g, (f32)color.b, (f32)color.a));

	quad_instances_count++;
}

void instance_renderer::render_quads(bool reset)
{
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, colors.size() * sizeof(glm::vec4), &colors[0]);

	glBindBuffer(GL_ARRAY_BUFFER, matrices_buffer);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model_matrices.size() * sizeof(glm::mat4), &model_matrices[0]);

	// draw instances
	glm::mat4 projection = glm::mat4(1.0f);
	f32 window_width = (f32)window::getInstance()->getWidth();
	f32 window_height = (f32)window::getInstance()->getHeight();

	projection = glm::ortho(0.0f, window_width, 0.0f, window_height, 0.0f, 100.0f);

	m_shader_program->bind();
	m_shader_program->set_mat4("u_projection", projection);
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, quad_instances_count * 6, GL_UNSIGNED_INT, 0, quad_instances_count);

	if (reset)
	{
		model_matrices.clear();
		colors.clear();
		quad_instances_count = 0;
	}
}