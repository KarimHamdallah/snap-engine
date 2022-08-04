#include "renderer.h"
#include <GLEW/glew.h>
#include <engine/logger/asserts.h>
#include <engine/display/window.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::shared_ptr<shader> renderer::m_default_shaderprogram;
std::shared_ptr<shader> renderer::m_sprite_shaderprogram;
render_object renderer::quad;
render_object renderer::line;

bool renderer::renderer_init(void)
{
	if (glewInit() != GLEW_OK)
	{
		LOG_ASSERT("glew Failed to initialize glew\n");
		return false;
	}
	LOG_INFO("OpenGL Renderer initialized Successfully");
	LOG_DEBUG("vendor: {}", (const char*)glGetString(GL_VENDOR));
	LOG_DEBUG("renderer: {}", (const char*)glGetString(GL_RENDERER));
	LOG_DEBUG("version: {}", (const char*)glGetString(GL_VERSION));
	
	m_default_shaderprogram = std::make_shared<shader>("assets/shaders/default_vertex.glsl", "assets/shaders/default_fragment.glsl");
	m_sprite_shaderprogram = std::make_shared<shader>("assets/shaders/sprite_vertex.glsl", "assets/shaders/sprite_fragment.glsl");

	int window_width = (int)window::getInstance()->getWidth();
	int window_height = (int)window::getInstance()->getHeight();
	glViewport(0, 0, window_width, window_height);


	// setup render_objects
	quad = rendererFactory::init_quad();
	line = rendererFactory::init_line();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void renderer::renderer_begin(color clear_color)
{
	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void renderer::renderer_end(SDL_Window * rendering_window)
{
	SDL_GL_SwapWindow(rendering_window);
}

void renderer::render_quad(glm::vec2 position, glm::vec2 scale, color color)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	f32 window_width = (f32)window::getInstance()->getWidth();
	f32 window_height = (f32)window::getInstance()->getHeight();
	//projection = glm::ortho(-0.5f * window_width, 0.5f * window_width, -0.5f * window_height, 0.5f * window_height, 0.0f, 100.0f);
	projection = glm::ortho(0.0f, window_width, 0.0f, window_height, 0.0f, 100.0f);

	m_default_shaderprogram->bind();

	m_default_shaderprogram->set_mat4("u_model", model);
	m_default_shaderprogram->set_mat4("u_projection", projection);
	m_default_shaderprogram->set_color("u_color", color);


	glBindVertexArray(quad.vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderer::render_line(glm::vec2 start, glm::vec2 end, color color, f32 lineWidth)
{
	glm::vec2 v = end - start;
	f32 line_vertices[]
	{
		0.0f, 0.0f, 0.0f,
		v.x, v.y, 0.0f
	};

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(start.x, start.y, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	f32 window_width = (f32)window::getInstance()->getWidth();
	f32 window_height = (f32)window::getInstance()->getHeight();

	projection = glm::ortho(0.0f, window_width, 0.0f, window_height, 0.0f, 100.0f);

	m_default_shaderprogram->bind();

	glLineWidth(lineWidth);

	m_default_shaderprogram->set_mat4("u_model", model);
	m_default_shaderprogram->set_mat4("u_projection", projection);
	m_default_shaderprogram->set_color("u_color", color);

	glBindVertexArray(line.vao);
	glBindBuffer(GL_ARRAY_BUFFER, line.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line_vertices), line_vertices);

	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void renderer::render_quadline(glm::vec2 position, glm::vec2 scale, color color, f32 lineWidth)
{
	glm::vec2 points[] = 
	{
		glm::vec2(position.x - scale.x * 0.5f, position.y - scale.x * 0.5f),
		glm::vec2(position.x + scale.x * 0.5f, position.y - scale.x * 0.5f),
		glm::vec2(position.x + scale.x * 0.5f, position.y + scale.x * 0.5f),
		glm::vec2(position.x - scale.x * 0.5f, position.y + scale.x * 0.5f),
	};

	for (size_t i = 0; i < 3; i++)
	{
		render_line(points[i], points[i + 1], color);
	}
	render_line(points[0], points[3], color);
}

void renderer::render_aabb(const AABB& _aabb, color color, f32 lineWidth)
{
	glm::vec2 scale = _aabb.half_scale * 2.0f;

	render_quadline(_aabb.position, scale, color, lineWidth);
}

texture renderer::creat_texture(const char * file_path, bool flip)
{
	texture tex;
	glGenTextures(1, &tex.id);
	glBindTexture(GL_TEXTURE_2D, tex.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	i32 width, height, n_channel;

	stbi_set_flip_vertically_on_load(flip);
	u8* data = stbi_load(file_path, &width, &height, &n_channel, 0);

	tex.width = width;
	tex.height = height;

	GLenum format = GL_RGB;
	GLenum internal_format = GL_RGB;

	if (n_channel == 4)
	{
		format = GL_RGBA;
		internal_format = GL_RGBA;
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		LOG_ERROR("Failed to load texture at location : {}", file_path);
	}

	stbi_image_free(data);
	
	return tex;
}

void renderer::render_sprite(texture * tex, const glm::vec2 & position, const glm::vec2 & scale, const color & color)
{
	if (!tex)
	{
		renderer::render_quad(position, scale, color);
		return;
	}

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	f32 window_width = (f32)window::getInstance()->getWidth();
	f32 window_height = (f32)window::getInstance()->getHeight();
	//projection = glm::ortho(-0.5f * window_width, 0.5f * window_width, -0.5f * window_height, 0.5f * window_height, 0.0f, 100.0f);
	projection = glm::ortho(0.0f, window_width, 0.0f, window_height, 0.0f, 100.0f);

	m_sprite_shaderprogram->bind();

	m_sprite_shaderprogram->set_mat4("u_model", model);
	m_sprite_shaderprogram->set_mat4("u_projection", projection);
	m_sprite_shaderprogram->set_color("u_color", color);
	m_sprite_shaderprogram->set_int("u_texture", 0);

	glBindTexture(GL_TEXTURE_2D, tex->id);
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(quad.vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

render_object rendererFactory::init_quad()
{
	u32 vao;
	u32 vbo;
	u32 ebo;

	f32 vertices[]
	{  // x      y     z     u    v
		 0.5f,  0.5f, 0.0f, 1.0f,1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 1.0f,0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f,0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f,1.0f  // top left 
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

	return render_object { vao,vbo,ebo };
}

render_object rendererFactory::init_line()
{
	u32 vao = 0;
	u32 vbo = 0;
	u32 ebo = 0;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(f32), nullptr, GL_DYNAMIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(f32), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return render_object{ vao,vbo,ebo };
}