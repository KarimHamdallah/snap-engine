#include "shader.h"
#include <engine/utils.h>
#include <GLEW/glew.h>

shader::shader(const std::string& vertexshader_filepath, const std::string& fragmentshader_filepath)
{
	gl_id vertexshader = compile_shader(GL_VERTEX_SHADER, get_file_content(vertexshader_filepath));
	gl_id fragmentshader = compile_shader(GL_FRAGMENT_SHADER, get_file_content(fragmentshader_filepath));

	link(vertexshader, fragmentshader);
}

shader::~shader()
{
	delete_id();
}

void shader::bind()
{
	glUseProgram(m_id);
}

void shader::unbind()
{
	glUseProgram(0);
}

void shader::delete_id()
{
	glDeleteProgram(m_id);
}

void shader::set_mat2(const std::string & name, const glm::mat2 & value)
{
	glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, false, &value[0][0]);
}

void shader::set_mat3(const std::string & name, const glm::mat3 & value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, false, &value[0][0]);
}

void shader::set_mat4(const std::string & name, const glm::mat4 & value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, false, &value[0][0]);
}

void shader::set_vec2(const std::string & name, const glm::vec2 & value)
{
	glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void shader::set_vec3(const std::string & name, const glm::vec3 & value)
{
	glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void shader::set_vec4(const std::string & name, const glm::vec4 & value)
{
	glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, glm::value_ptr(value));
}

void shader::set_color(const std::string & name, const color & value)
{
	glUniform4f(glGetUniformLocation(m_id, name.c_str()), value.r, value.g, value.b, value.a);
}

void shader::set_int(const std::string & name, const i32 & value)
{
	glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void shader::set_float(const std::string & name, const f32 & value)
{
	glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

gl_id shader::compile_shader(u32 type, const std::string & shader_file_content)
{
	i32 success;
	const i32 buffsize = 1024;
	char info_log[buffsize];

	u32 _shader;
	const char* shader_source = shader_file_content.c_str();

	_shader = glCreateShader(type);
	glShaderSource(_shader, 1, &shader_source, nullptr);
	glCompileShader(_shader);

	glGetShaderiv(_shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		std::string shader_type_Str = (type == GL_VERTEX_SHADER ? "vertex" : "fragment");
		glGetShaderInfoLog(_shader, buffsize, nullptr, info_log);

		LOG_ERROR("failed to compile {0} shader :\n {1}", shader_type_Str, info_log);
	}

	return _shader;
}

void shader::link(gl_id vertex_shader, gl_id fragment_shader)
{
	i32 success;
	const i32 buffsize = 1024;
	char info_log[buffsize];

	m_id = glCreateProgram();
	glAttachShader(m_id, vertex_shader);
	glAttachShader(m_id, fragment_shader);

	glLinkProgram(m_id);

	glGetShaderiv(m_id, GL_LINK_STATUS, &success);


	if (!success)
	{
		glGetProgramInfoLog(m_id, buffsize, nullptr, info_log);

		LOG_ERROR("failed to link shader program :\n {}", info_log);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}