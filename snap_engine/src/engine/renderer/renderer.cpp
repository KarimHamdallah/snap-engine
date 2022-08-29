#include "renderer.h"
#include <GLEW/glew.h>
#include <engine/logger/asserts.h>
#include <engine/display/window.h>
#include <engine/math/math.h>
#include <engine/input/input.h>

std::shared_ptr<shader> renderer::m_default_shaderprogram;
std::shared_ptr<shader> renderer::m_smoothcircle_shaderprogram;
std::shared_ptr<shader> renderer::m_smoothline_shaderprogram;
render_object renderer::quad;
render_object renderer::quad_bottomleft;
render_object renderer::quad_topleft;
render_object renderer::quad_bottom;
render_object renderer::quad_top;
render_object renderer::line;
render_object renderer::smoothcircle;
render_object renderer::smoothline;

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
	
	// init shader programs
	m_default_shaderprogram = std::make_shared<shader>("assets/shaders/default_vertex.glsl", "assets/shaders/default_fragment.glsl");
	m_smoothcircle_shaderprogram = std::make_shared<shader>("assets/shaders/smoothcircle_vertex.glsl", "assets/shaders/smoothcircle_fragment.glsl");
	m_smoothline_shaderprogram = std::make_shared<shader>("assets/shaders/smoothline_vertex.glsl", "assets/shaders/smoothline_fragment.glsl");

	// set view port
	int window_width = (i32)window::getInstance()->getWidth();
	int window_height = (i32)window::getInstance()->getHeight();
	glViewport(0, 0, window_width, window_height);

	// set projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, (f32)window_width, 0.0f, (f32)window_height, 0.0f, 100.0f);

	m_default_shaderprogram->bind();
	m_default_shaderprogram->set_mat4("u_projection", projection);

	m_smoothcircle_shaderprogram->bind();
	m_smoothcircle_shaderprogram->set_mat4("u_projection", projection);

	m_smoothline_shaderprogram->bind();
	m_smoothline_shaderprogram->set_mat4("u_projection", projection);

	// setup render_objects
	quad = rendererFactory::init_quad();
	quad_bottomleft = rendererFactory::init_quad(pivot::bottom_left);
	quad_topleft = rendererFactory::init_quad(pivot::top_left);
	quad_bottom = rendererFactory::init_quad(pivot::bottom);
	quad_top = rendererFactory::init_quad(pivot::top);
	line = rendererFactory::init_line();
	smoothcircle = rendererFactory::init_smoothcircle();
	smoothline = rendererFactory::init_smoothline();

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

void renderer::render_quad(glm::vec2 position, glm::vec2 scale, color color, pivot pivot_point)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	m_default_shaderprogram->bind();

	m_default_shaderprogram->set_mat4("u_model", model);
	m_default_shaderprogram->set_color("u_color", color);

	switch (pivot_point)
	{
	case pivot::center:
		glBindVertexArray(quad.vao);
		break;
	case pivot::bottom_left:
		glBindVertexArray(quad_bottomleft.vao);
		break;
	case pivot::top_left:
		glBindVertexArray(quad_topleft.vao);
		break;
	case pivot::bottom:
		glBindVertexArray(quad_bottom.vao);
		break;
	case pivot::top:
		glBindVertexArray(quad_top.vao);
		break;
	}
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderer::render_quad(glm::vec2 position, glm::vec2 scale, f32 rotation, color color)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

	m_default_shaderprogram->bind();

	m_default_shaderprogram->set_mat4("u_model", model);
	m_default_shaderprogram->set_color("u_color", color);


	glBindVertexArray(quad.vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderer::render_line(const glm::vec2& start, const glm::vec2& end, const color& color, f32 lineWidth)
{
	glm::vec2 v = end - start;
	f32 line_vertices[]
	{
		0.0f, 0.0f, 0.0f,
		v.x, v.y, 0.0f
	};

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(start.x, start.y, 0.0f));

	m_default_shaderprogram->bind();

	glLineWidth(lineWidth);

	m_default_shaderprogram->set_mat4("u_model", model);
	m_default_shaderprogram->set_color("u_color", color);

	glBindVertexArray(line.vao);
	glBindBuffer(GL_ARRAY_BUFFER, line.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(line_vertices), line_vertices);

	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void renderer::render_wideline(const glm::vec2 & start, const glm::vec2 & end, const color & color, f32 lineWidth)
{
	if (start.x == end.x && start.y == end.y)
		renderer::render_quad(start, glm::vec2(lineWidth), color); // point
	else if (start.y == end.y)
		renderer::render_quad(start, glm::vec2(end.x - start.x, lineWidth), color, pivot::bottom_left); // horizontal line
	else if (start.x == end.x)
		renderer::render_quad(start, glm::vec2(lineWidth, end.y - start.y), color, pivot::bottom_left); // vertical line
	else
	{
	
		/*
		glm::vec2 min, max, scale, position;
		min.x = std::min(start.x, end.x);
		min.y = std::min(start.y, end.y);
		max.x = std::max(start.x, end.x);
		max.y = std::max(start.y, end.y);
		scale.x = max.x - min.x;
		scale.y = max.y - min.y;
		position.x = min.x + scale.x * 0.5f;
		position.y = min.y + scale.y * 0.5f;
		*/

		glm::vec2 resolution = glm::vec2(window::getInstance()->getWidth(),
			window::getInstance()->getHeight());
		glm::vec2 windowCneter = resolution * 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(windowCneter.x, windowCneter.y, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(resolution.x, resolution.y, 1.0f));


		m_smoothline_shaderprogram->bind();

		m_smoothline_shaderprogram->set_mat4("u_model", model);
		m_smoothline_shaderprogram->set_color("u_color", color);
		m_smoothline_shaderprogram->set_vec2("u_resolution", resolution);
		m_smoothline_shaderprogram->set_vec2("u_startPoint", start);
		m_smoothline_shaderprogram->set_vec2("u_endPoint", end);
		m_smoothline_shaderprogram->set_float("u_thickness", lineWidth);

		glBindVertexArray(smoothline.vao);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void renderer::render_quadline(glm::vec2 position, glm::vec2 scale, color color, f32 lineWidth)
{
	glm::vec2 points[] = 
	{
		glm::vec2(position.x - scale.x * 0.5f, position.y - scale.y * 0.5f),
		glm::vec2(position.x + scale.x * 0.5f, position.y - scale.y * 0.5f),
		glm::vec2(position.x + scale.x * 0.5f, position.y + scale.y * 0.5f),
		glm::vec2(position.x - scale.x * 0.5f, position.y + scale.y * 0.5f),
	};

	for (size_t i = 0; i < 4; i++)
	{
		render_wideline(points[i], points[(i + 1) % 4], color);
	}
}

void renderer::render_aabb(const AABB& _aabb, color color, f32 lineWidth)
{
	glm::vec2 scale = _aabb.half_scale * 2.0f;

	render_quadline(_aabb.center, scale, color, lineWidth);
}

void renderer::render_smoothcircle(const glm::vec2& position, const glm::vec2& scale, const color& color, f32 thickness, f32 fade)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

	fade = math::clampf(fade, 0.0f, 1.0f);
	thickness = math::clampf(thickness, 0.0f, 1.0f);

	m_smoothcircle_shaderprogram->bind();

	m_smoothcircle_shaderprogram->set_mat4("u_model", model);
	m_smoothcircle_shaderprogram->set_color("u_color", color);
	m_smoothcircle_shaderprogram->set_float("u_fade", fade);
	m_smoothcircle_shaderprogram->set_float("u_thickness", thickness);


	glBindVertexArray(smoothcircle.vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderer::render_smoothcircle(const glm::vec2 & position, f32 diameter, const color & color, f32 thickness)
{
	// 500 here as const raduis that can perfectly use fade 0.005f to get smooth edges
	// so we get ratio of 500 and raduis and this ratio scaled by fade of 500
	f32 fade = (500.0f / diameter) * 0.005f;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
	model = glm::scale(model, glm::vec3(diameter, diameter, 1.0f));
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	thickness = math::clampf(thickness, 0.0f, 1.0f);

	m_smoothcircle_shaderprogram->bind();

	m_smoothcircle_shaderprogram->set_mat4("u_model", model);
	m_smoothcircle_shaderprogram->set_color("u_color", color);
	m_smoothcircle_shaderprogram->set_float("u_fade", fade);
	m_smoothcircle_shaderprogram->set_float("u_thickness", thickness);


	glBindVertexArray(smoothcircle.vao);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void renderer::render_polygonline(const std::vector<glm::vec2>& vertices, color color, f32 lineWidth)
{
	for (size_t i = 0; i < vertices.size(); i++)
	{
		glm::vec2 va = vertices[i];
		glm::vec2 vb = vertices[(i + 1) % vertices.size()];
		render_wideline(va, vb, color, lineWidth);
	}
}

void renderer::render_polygonline(glm::vec2 * vertices, u32 vertices_count, color color, f32 lineWidth)
{
	for (size_t i = 0; i < vertices_count; i++)
	{
		glm::vec2 va = vertices[i];
		glm::vec2 vb = vertices[(i + 1) % vertices_count];
		render_wideline(va, vb, color, lineWidth);
	}
}

void renderer::free_memory()
{
	glDeleteVertexArrays(1, &quad.vao);
	glDeleteVertexArrays(1, &quad_bottom.vao);
	glDeleteVertexArrays(1, &quad_bottomleft.vao);
	glDeleteVertexArrays(1, &quad_top.vao);
	glDeleteVertexArrays(1, &quad_topleft.vao);
	glDeleteVertexArrays(1, &smoothcircle.vao);
	glDeleteVertexArrays(1, &line.vao);
	glDeleteBuffers(1, &smoothline.vao);

	glDeleteBuffers(1, &quad.vbo);
	glDeleteBuffers(1, &quad_bottom.vbo);
	glDeleteBuffers(1, &quad_bottomleft.vbo);
	glDeleteBuffers(1, &quad_top.vbo);
	glDeleteBuffers(1, &quad_topleft.vbo);
	glDeleteBuffers(1, &smoothcircle.vbo);
	glDeleteBuffers(1, &line.vbo);
	glDeleteBuffers(1, &smoothline.vbo);

	glDeleteBuffers(1, &quad.ebo);
	glDeleteBuffers(1, &quad_bottom.ebo);
	glDeleteBuffers(1, &quad_bottomleft.ebo);
	glDeleteBuffers(1, &quad_top.ebo);
	glDeleteBuffers(1, &quad_topleft.ebo);
	glDeleteBuffers(1, &smoothcircle.ebo);
	glDeleteBuffers(1, &line.ebo);
	glDeleteBuffers(1, &smoothline.ebo);
}

render_object rendererFactory::init_quad(pivot pivot_point)
{
	u32 vao;
	u32 vbo;
	u32 ebo;

	const u32 vertices_count = 4;
	const u32 vertex_stride = 5;

	f32* vertices_ptr = new f32[vertices_count * vertex_stride];
	// center
	if (pivot_point == pivot::center)
	{
		f32 vertices[] =
		{
			// positions        // texture coords
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,   // top right
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // bottom left
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f    // top left 
		};
		vertices_ptr = &vertices[0];
	}
	if (pivot_point == pivot::bottom_left)
	{
		f32 vertices[] =
		{    // x      y     z  texture coords
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
			 1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
			 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
			 0.0f,  1.0f, 0.0f, 0.0f, 1.0f   // top left 
		};
		vertices_ptr = &vertices[0];
	}
	if (pivot_point == pivot::top_left)
	{
		f32 vertices[] =
		{    // x      y     z  texture coords
			 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,  // top right
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
			 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom left
			 0.0f,  0.0f, 0.0f, 0.0f, 1.0f   // top left
		};
		vertices_ptr = &vertices[0];
	}
	if (pivot_point == pivot::bottom)
	{
		f32 vertices[] =
		{
			// positions        // texture coords
			 0.5f,  1.0f, 0.0f, 1.0f, 1.0f,   // top right
			 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,   // bottom right
			-0.5f,  0.0f, 0.0f, 0.0f, 0.0f,   // bottom left
			-0.5f,  1.0f, 0.0f, 0.0f, 1.0f    // top left 
		};
		vertices_ptr = &vertices[0];
	}
	if (pivot_point == pivot::top)
	{
		f32 vertices[] =
		{
			// positions        // texture coords
			 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,   // top right
			 0.5f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
			-0.5f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
			-0.5f,  0.0f, 0.0f, 0.0f, 0.0f,   // top left
		};
		vertices_ptr = &vertices[0];
	}
	

	u32 indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_count * vertex_stride * sizeof(f32), vertices_ptr, GL_STATIC_DRAW);

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

render_object rendererFactory::init_smoothcircle()
{
	u32 vao;
	u32 vbo;
	u32 ebo;


	f32 vertices[]
	{  // x      y     z     local positions
		 0.5f,  0.5f, 0.0f,  1.0f,  1.0f,  // top right
		 0.5f, -0.5f, 0.0f,  1.0f, -1.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, -1.0f, -1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, -1.0f,  1.0f,  // top left 
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
	// local position attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);

	return render_object{ vao,vbo,ebo };
}

render_object rendererFactory::init_smoothline()
{
	u32 vao;
	u32 vbo;
	u32 ebo;


	f32 vertices[]
	{  // x      y     z     local positions
		 0.5f,  0.5f, 0.0f,  1.0f,  1.0f,  // top right
		 0.5f, -0.5f, 0.0f,  1.0f,  0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f,  0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,  0.0f,  1.0f,  // top left 
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
	// local position attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(f32)));
	glEnableVertexAttribArray(1);

	return render_object{ vao,vbo,ebo };
}